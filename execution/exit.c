#include "execution.h"

void	exit_shell(t_env_list **list_var, char **envp)
{
	t_env_list	*head;
	t_env_list	*tmp;
	int			i;

	i = 0;
	while (envp[i])
	{
		free(envp[i]);
		i++;
	}
	free(envp);
	head = *list_var;
	while (head != NULL)
	{
		tmp = head;
		if (tmp)
		{
			free(tmp->name);
			free(tmp->value);
			free(tmp);
		}
		head = head->next;
	}
	ft_printf("exit\n");
	exit(1);
}
