/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skock <skock@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 14:56:57 by skock             #+#    #+#             */
/*   Updated: 2025/02/06 15:05:40 by skock            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	free_tab(char **tab)
{
	int	i;

	i = 0;
	while(tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

void	free_pipex(t_pipex *pipex)
{
	if (pipex->argv_cmd_1)
		free_tab(pipex->argv_cmd_1);
	if (pipex->argv_cmd_2)
		free_tab(pipex->argv_cmd_2);
	if (pipex->path_cmd_1)
		free(pipex->path_cmd_1);
	if (pipex->path_cmd_2)
		free(pipex->path_cmd_2);
	if (pipex->env)
		free_tab(pipex->env);
	return ;
}