/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skock <skock@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 14:31:11 by skock             #+#    #+#             */
/*   Updated: 2025/02/05 14:05:06 by skock            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	fill_env(t_pipex *pipex, char **env)
{
	int		i = 0;
	char	**path;

	i = -1;
	path = NULL;
	while(env[++i])
	{
		if (ft_strncmp(env[i], "PATH=", 5) == 0)
		{
			path = ft_split(env[i], ':');
			break ;
		}
	}
	path[0] = ft_substr(path[0], 5, ft_strlen(path[0]) - 5);
	i = -1;
	while(path[++i])
		path[i] = ft_strjoin(path[i], "/");
	pipex->env = path;
}

void	access_cmd_1(t_pipex *pipex)
{
	int		i = 0;
	bool	ok;
	char	**tab;

	ok = false;
	tab = ft_split(pipex->av[2], ' ');
	while (pipex->env[i])
	{
		if (!access(ft_strjoin(pipex->env[i], tab[0]), X_OK))
		{
			ok = true;
			break ;
		}
		i++;
	}
	if (ok == false)
		pipex->path_cmd_1 = NULL;
	else
		pipex->path_cmd_1 = ft_strjoin(pipex->env[i], tab[0]);
}

void	access_cmd_2(t_pipex *pipex)
{
	int		i = 0;
	bool	ok;
	char	**tab;

	ok = false;
	tab = ft_split(pipex->av[3], ' ');
	while (pipex->env[i])
	{
		if (!access(ft_strjoin(pipex->env[i], tab[0]), X_OK))
		{
			ok = true;
			break ;
		}
		i++;
	}
	if (ok == false)
		pipex->path_cmd_2 = NULL;
	else
		pipex->path_cmd_2 = ft_strjoin(pipex->env[i], tab[0]);
}

void	create_file(t_pipex *pipex)
{
	pipex->infile_fd = open(pipex->av[1], O_RDONLY, 0644);
	// if (pipex->infile_fd < 0)
		// update_error();
	pipex->outfile_fd = open(pipex->av[4], O_CREAT | O_RDWR | O_TRUNC, 0644);
	// if (pipex->infile_fd < 0)
		// update_error();
}

void	child_process(t_pipex *pipex, int *fd, char **env)
{
	if (pipex->flag == 1)
	{
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
	}
	else
	{
		dup2(pipex->infile_fd, STDIN_FILENO);
		close(pipex->infile_fd);
	}
	if (pipex->flag == 1)
	{
		dup2(pipex->outfile_fd, STDOUT_FILENO);
		close(pipex->outfile_fd);
	}
	else
	{
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
	}
	if (pipex->flag == 0)
	{
		execve(pipex->path_cmd_1, pipex->argv_cmd_1, env);
		exit(1);
	}
	else
	{
		execve(pipex->path_cmd_2, pipex->argv_cmd_2, env);
		exit(1);
	}
}

void	parent_process(t_pipex *pipex, int *fd)
{
	if (pipex->flag == 0)
		close(fd[1]);
	else
	{
		close(fd[0]);
		close(fd[1]);
	}
}

void	exec(t_pipex *pipex, char **env)
{
	int		fd[2];
	int		loop;
	pid_t	pid;

	pipe(fd);
	loop = 2;
	pipex->flag = 0;
	pipex->argv_cmd_1 = ft_split(pipex->av[2], ' ');
	free(pipex->argv_cmd_1[0]);
	pipex->argv_cmd_1[0] = ft_strdup(pipex->path_cmd_1);
	pipex->argv_cmd_2 = ft_split(pipex->av[3], ' ');
	for (int j = 0; pipex->argv_cmd_1[j]; j++)
		printf("%s\n", pipex->argv_cmd_1[j]);
	while (loop > 0)
	{
		pid = fork();
		if (pid == 0)
			child_process(pipex, fd, env);
		else
			parent_process(pipex, fd);
		pipex->flag = 1;
		loop--;
	}
	return ;
}

int	main(int ac, char **av, char **env)
{
	t_pipex	pipex;

	pipex.ac = ac;
	pipex.av = av;
	if (ac == 5)
	{
		fill_env(&pipex, env);
		access_cmd_1(&pipex);
		access_cmd_2(&pipex);
		create_file(&pipex);
		exec(&pipex, env);
		return (1);
	}
	else
		return (0);

}
