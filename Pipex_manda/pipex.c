/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skock <skock@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 14:31:11 by skock             #+#    #+#             */
/*   Updated: 2025/02/03 14:54:18 by skock            ###   ########.fr       */
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

	ok = false;
	while (pipex->env[i])
	{
		if (!access(ft_strjoin(pipex->env[i], pipex->av[2]), X_OK))
		{
			ok = true;
			break ;
		}
		i++;
	}
	if (ok == false)
		pipex->path_cmd_1 = NULL;
	else
		pipex->path_cmd_1 = ft_strjoin(pipex->env[i], pipex->av[2]);
}

void	access_cmd_2(t_pipex *pipex)
{
	int		i = 0;
	bool	ok;

	ok = false;
	while (pipex->env[i])
	{
		if (!access(ft_strjoin(pipex->env[i], pipex->av[3]), X_OK))
		{
			ok = true;
			break ;
		}
		i++;
	}
	if (ok == false)
		pipex->path_cmd_2 = NULL;
	else
		pipex->path_cmd_2 = ft_strjoin(pipex->env[i], pipex->av[3]);
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
	close(fd[0]);
	dup2(pipex->infile_fd, STDIN_FILENO);
	if (!pipex->path_cmd_2)
			dup2(pipex->outfile_fd, STDOUT_FILENO);
	else
			dup2(fd[1], STDOUT_FILENO);
	close(fd[1]);
	close(pipex->infile_fd);
	if (pipex->outfile_fd > 2)
			close(pipex->outfile_fd);
	if (pipex->flag == 0)
	{
		pipex->flag = 1;
		execve(pipex->path_cmd_1, pipex->argv, env);
	}
	else
		execve(pipex->path_cmd_2, pipex->argv, env);

}

void	parent_process(t_pipex *pipex, int *fd)
{
	close(fd[1]);
	close(pipex->infile_fd);
	pipex->infile_fd = dup(fd[0]);
	close(fd[0]);
}

void	exec(t_pipex *pipex, char **env)
{
	int		fd[2];
	int		loop;
	pid_t	pid;

	pipe(fd);
	loop = 2;
	pipex->flag = 0;
	pipex->argv[0] = pipex->av[2];
	pipex->argv[1] = pipex->av[3];
	pipex->argv[2] = NULL;
	while (loop > 0)
	{
		pid = fork();
		if (pid == 0)
			child_process(pipex, fd, env);
		else
			parent_process(pipex, fd);
		loop--;
	}
	printf("exit loop\n");
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
