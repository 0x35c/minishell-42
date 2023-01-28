/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adamiens <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 13:50:00 by adamiens          #+#    #+#             */
/*   Updated: 2023/01/28 17:57:14 by ulayus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_sh_state	g_sh_state;

void	create_exit_env(t_env_list **list_var)
{
	char	**cmd;

	cmd = ft_split("export ?=0", ' ');
	if (cmd == NULL)
		exit_shell(list_var, NULL);
	main_export(cmd, list_var);
	ft_free_strs(cmd);
}

void	start_shell(char *line, t_cmd *cmd, t_env_list *list_var, char **envp)
{
	if (ft_strlen(line) != 0)
	{
		cmd = parsing(line, list_var);
		signal(SIGQUIT, handle_sigquit);
		if (cmd != NULL)
			execution(cmd, envp, &list_var);
	}
	if (line && *line)
		add_history(line);
	if (line)
		free(line);
	if (envp)
		ft_free_strs(envp);
}

int	main(int ac, char **av, char **envp)
{
	t_env_list	*list_var;
	char		*line;
	t_cmd		*cmd;
	char		*prompt;

	(void)av;
	if (ac != 1)
		return (1);
	cmd = NULL;
	list_var = create_env_list(envp);
	create_exit_env(&list_var);
	signal(SIGINT, handle_sigint);
	while (1)
	{
		signal(SIGQUIT, SIG_IGN);
		g_sh_state.state = PROMPT;
		prompt = create_prompt();
		line = readline(prompt);
		envp = env_export(list_var);
		free(prompt);
		if (line == NULL)
			exit_shell(&list_var, envp);
		start_shell(line, cmd, list_var, envp);
	}
	return (0);
}