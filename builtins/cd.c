#include "builtins.h"

void	*main_cd(char **args, t_env_list **list_var)
{
	if (chdir(args[1]))
	{
		ft_printf("bash: cd: %s: No such file or directory\n", args[1]);
		return (NULL);
	}
	else
		return (list_var);
}
