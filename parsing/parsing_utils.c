#include "parsing.h"

void	free_cmd(cmd_t *cmd)
{
	int	i;
	int	j;

	i = 0;
	while (cmd[i].cmd[0] != NULL)
	{
		j = 0;
		while (cmd[i].cmd[j] != NULL)
		{
			free(cmd[i].cmd[j]);
			j++;
		}
		ft_printf("free %dx", i);
		free(cmd[i].cmd);
		i++;
	}
	free(cmd);
}