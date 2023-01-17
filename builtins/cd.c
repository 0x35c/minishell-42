#include "builtins.h"

void	search_replace_pwd(char *pwd, int flag, t_env_list **list_var)
{
	t_env_list	*tmp;

	tmp = *list_var;
	while (tmp->next)
	{
		if (flag == PWD)
		{
			if (ft_strcmp("PWD", tmp->name) == false)
			{
				free(tmp->value);
				tmp->value = pwd;
			}
		}
		else if (flag == OLDPWD)
		{
			if (ft_strcmp("OLDPWD", tmp->name) == false)
			{
				free(tmp->value);
				tmp->value = pwd;
			}
		}
		tmp = tmp->next;
	}
}

char	*get_current_pwd(t_env_list **list_var)
{
	t_env_list	*tmp;

	tmp = *list_var;
	while (tmp->next)
	{
		if (ft_strcmp("PWD", tmp->name) == false)
			return (tmp->value);
		tmp = tmp->next;
	}
	return (NULL);
}

void	*main_cd(char **args, t_env_list **list_var)
{
	char	*old_pwd;
	char	*pwd;

	old_pwd = ft_calloc(4096, sizeof(char));
	if (old_pwd == NULL)
		return (NULL);
	old_pwd = getcwd(old_pwd, 4096);
	if (chdir(args[1]))
	{
		ft_printf("bash: cd: %s: No such file or directory\n", args[1]);
		return (NULL);
	}
	else
	{
		pwd = ft_calloc(4096, sizeof(char));
		if (pwd == NULL)
			return (NULL);
		getcwd(pwd, 4096);
		search_replace_pwd(old_pwd, OLDPWD, list_var);
		search_replace_pwd(pwd, PWD, list_var);
		return (list_var);
	}
}
