#include "parsing.h"
#include <stdio.h>

void	begin_offset(t_cmd **cmd, char *line)
{
	*cmd = list_new(NULL, NULL);
	if (line[0] && line[0] == '<' && line[1] && line[1] == '<')
		(*cmd)->token = HEREDOC;
	else if (line[0] && line[0] == '>' && line[1] && line[1] == '>')
		(*cmd)->token = APPEND;
	else if (line[0] && line[0] == '>')
		(*cmd)->token = OUT;
	else if (line[0] && line[0] == '<')
		(*cmd)->token = IN;
}

/********************************************************/
/*À appliquer sur toutes les lignes:					*/
/*	separe la ligne par delimiteurs (>, >>, |, <<, <)	*/	
/*	devient des tokens, stocker dans tab de cmd			*/		
/*	assigne le delim qui a cut le token a cmd id		*/
/********************************************************/

t_cmd	*get_cmd(char *line)
{
	t_cmd	*cmd;
	t_token	*token;

	cmd = NULL;
	if (line[0] && (line[0] == '<' || line[0] == '>'))
	{
		begin_offset(&cmd, line);
		if (line[0] && line[1] && ((line[0] == '<' && line[1] == '<')
				|| (line[0] == '>' && line[1] == '>')))
			line += 2;
		else if (line[0] && (line[0] == '>' || line[0] == '<'))
			line++;
		token = str_get_token(line, ">|<");
		cmd->next = list_new(split_token(token->line, ' '), cmd);
		cmd = cmd->next;
	}
	else
	{
		token = str_get_token(line, ">|<");
		cmd = list_new(split_token(token->line, ' '), NULL);
	}
	cmd->next = list_new(NULL, cmd);
	cmd = cmd->next ;
	cmd->token = token->id;
	while (token->line != NULL)
	{
		free(token->line);
		free(token);
		token = str_get_token(NULL, ">|<");
		if (token->line == NULL)
			break ;
		cmd->next = list_new(split_token(token->line, ' '), cmd);
		cmd = cmd->next ;
		cmd->next = list_new(NULL, cmd);
		cmd = cmd->next ;
		cmd->token = token->id;
	}
	free(token);
	while (cmd->prev != NULL)
		cmd = cmd->prev;
	return (cmd);
}

/********************************************************/
/*	Check if there are some delim						*/
/*	at the beginning or the at end of the line			*/
/********************************************************/		

int	check_char(char c)
{
	if (c == '|'
		|| c == '>'
		|| c == '<')
		return (1);
	else
		return (0);
}

/********************************************************/
/*	Parsing:											*/
/*	  separe la ligne avec delimiteurs(become tokens)	*/
/*	  split ces tokens par les espaces (cmd & arg)		*/
/********************************************************/

#include <stdio.h>
t_cmd	*parsing(char *line)
{
	t_cmd	*begin;
	char	*current_line;

	current_line = ft_strtrim(line, " ");
	if (ft_strlen(current_line) == 0)
	{
		free(current_line);
		return (NULL);
	}
	if ((check_char(current_line[0]) == 1
			&& (ft_strlen(current_line) == 1 || current_line[0] == '|'))
		|| check_char(current_line[ft_strlen(current_line) - 1]) == 1)
	{
		ft_putstr_fd("syntax error near unexpected token `newline'\n", 2);
		return (NULL);
	}
	begin = get_cmd(current_line);
	while (begin->next)
	{
		if (begin->token == 0 && begin->prev && begin->prev->token != 0 && begin->prev->token != PIPE)
			begin->token = FILES;
		begin = begin->next;
	}
	while (begin->prev != NULL)
		begin = begin->prev;
	while (begin->next)
	{
		if (begin->token == 0)
			begin->token = CMD;
		begin = begin->next;
	}
	while (begin->prev != NULL)
		begin = begin->prev;
	free(current_line);
	/*t_cmd	*tmp;
	tmp = begin;
	while (tmp)
	{
		int	i = 0;
		if (tmp->cmd != NULL)
		{
			while (tmp->cmd[i] != NULL)
			{
				printf("cmd[%d] = %s token : %d\n", i, tmp->cmd[i], tmp->token);
				i++;
			}
		}
		else
			printf("token : %d\n", tmp->token);
		tmp = tmp->next;
	}*/
	return (begin);
}
