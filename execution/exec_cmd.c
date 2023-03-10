/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulayus <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/01 13:17:48 by ulayus            #+#    #+#             */
/*   Updated: 2023/02/11 14:52:43 by adamiens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	special_dup2(t_exec *exec, t_cmd **cmd, int tab_pipe[2][2])
{
	if ((*cmd)->prev
		&& ((*cmd)->prev->token == FILES || (*cmd)->prev->token == IN
			|| (*cmd)->prev->token == HEREDOC || (*cmd)->prev->token == OUT
			|| (*cmd)->prev->token == APPEND))
	{
		(*cmd) = (*cmd)->prev;
		while ((*cmd)->prev && ((*cmd)->token == FILES || (*cmd)->token == IN
				|| (*cmd)->token == HEREDOC || (*cmd)->token == OUT
				|| (*cmd)->token == APPEND))
			(*cmd) = (*cmd)->prev;
		open_file(exec, (*cmd), 0, tab_pipe);
		while ((*cmd)->next
			&& (*cmd)->token != CMD && (*cmd)->token != BUILTINS)
			(*cmd) = (*cmd)->next;
	}
}

void	dup2_manager(t_exec *exec, int tab_pipe[2][2], t_cmd *cmd)
{
	special_dup2(exec, &cmd, tab_pipe);
	if (cmd->next && (cmd->token == FILES || cmd->token == CMD
			|| cmd->token == 0 || cmd->token == BUILTINS))
		cmd = cmd->next;
	if (cmd->token == HEREDOC
		|| cmd->token == IN
		|| cmd->token == OUT
		|| cmd->token == APPEND)
		open_file(exec, cmd, 0, tab_pipe);
	if (cmd->prev && (cmd->prev->token == FILES || cmd->prev->token == CMD
			|| cmd->prev->token == 0 || cmd->prev->token == BUILTINS))
		cmd = cmd->prev;
	if (cmd->prev && cmd->prev->token == PIPE
		&& cmd->next && cmd->next->token != IN && cmd->next->token != HEREDOC)
		dup2(tab_pipe[(exec->nb_fork - 1) % 2][0], 0);
	while (cmd->next)
	{
		if (cmd->token == PIPE
			|| cmd->token == 0 || cmd->token == OUT || cmd->token == APPEND)
			break ;
		cmd = cmd->next;
	}
	if (cmd && cmd->token == PIPE)
		dup2(tab_pipe[exec->nb_fork % 2][1], 1);
}

void	inside_fork(t_exec *exec, t_cmd *cmd, int tab_pipe[2][2])
{
	int	ret;

	ret = 0;
	dup2_manager(exec, tab_pipe, cmd);
	while (cmd->next && cmd->token != CMD
		&& cmd->token != BUILTINS && cmd->token != PIPE)
		cmd = cmd->next;
	if (cmd->token == CMD)
	{
		close_pipe(tab_pipe);
		ret = execve(cmd->cmd[0], cmd->cmd, exec->envp);
	}
	else if (cmd->token == BUILTINS)
		exec_builtins(cmd, &ret, tab_pipe, exec);
	else
		close_pipe(tab_pipe);
	exec_free(exec, cmd, ret);
}

void	apply_execution(t_exec *exec, t_cmd *cmd, int tab_pipe[2][2])
{
	if (cmd->token != UN_FORK)
		open_pipe(tab_pipe, exec->nb_fork);
	exec->tab_pid[exec->nb_fork] = fork();
	if (exec->tab_pid[exec->nb_fork] == -1)
	{
		perror("Error");
		g_sh_state.exit_code = 127;
	}
	else if (exec->tab_pid[exec->nb_fork] == 0)
		inside_fork(exec, cmd, tab_pipe);
}

void	exec_cmd(t_exec *exec, t_cmd *cmd, int tab_pipe[2][2])
{
	while (cmd->next && cmd->token != CMD
		&& cmd->token != BUILTINS && cmd->token != PIPE)
		cmd = cmd->next;
	if (cmd->cmd != NULL && cmd->token == CMD
		&& (ft_strcmp(cmd->cmd[0], "echo") == 0
			|| ft_strcmp(cmd->cmd[0], "pwd") == 0
			|| ft_strcmp(cmd->cmd[0], "export") == 0
			|| ft_strcmp(cmd->cmd[0], "unset") == 0
			|| ft_strcmp(cmd->cmd[0], "env") == 0
			|| ft_strcmp(cmd->cmd[0], "cd") == 0
			|| ft_strcmp(cmd->cmd[0], "exit") == 0))
		builtins_selection(cmd, exec, tab_pipe);
	if (verif_args(cmd, 1))
		return ;
	if (cmd->cmd && cmd->token == CMD && cmd->token != BUILTINS
		&& access(cmd->cmd[0], X_OK) != 0)
	{
		if (cmd->cmd[0][0] == '\0')
			return ;
		get_cmd_path(cmd->cmd, exec->envp);
		if (verif_args(cmd, 2))
			return ;
	}
	apply_execution(exec, cmd, tab_pipe);
	exec->nb_fork++;
}
