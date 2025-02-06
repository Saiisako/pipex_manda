/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skock <skock@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 14:31:11 by skock             #+#    #+#             */
/*   Updated: 2025/02/06 16:29:28 by skock            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

char	*ft_strjoin_env(char *s1, char const *s2)
{
	char	*str;
	size_t	i;
	size_t	j;

	str = (char *)malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (!str)
		return (NULL);
	i = 0;
	j = 0;
	if (s1)
	{
		while (s1[i])
			str[j++] = s1[i++];
	}
	i = 0;
	if (s2)
	{
		while (s2[i])
			str[j++] = s2[i++];
	}
	str[j] = '\0';
	free(s1);
	return (str);
}

void	fill_env(t_pipex *pipex, char **env)
{
	int		i = 0;
	char	**path;
	char	*tmp;

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
	tmp = ft_substr(path[0], 5, ft_strlen(path[0]) - 5);
	free(path[0]);
	path[0] = ft_strdup(tmp);
	free(tmp);
	i = -1;
	while (path[++i])
		path[i] = ft_strjoin_env(path[i], "/");
	pipex->env = path;
}

void	access_cmd_1(t_pipex *pipex)
{
	int		i = 0;
	bool	ok;
	char	**tab;
	char	*tmp;

	ok = false;
	tab = ft_split(pipex->av[2], ' ');
	while (pipex->env[i])
	{
		tmp = ft_strjoin(pipex->env[i], tab[0]);
		if (!access(tmp, X_OK))
		{
			free(tmp);
			ok = true;
			break ;
		}
		free(tmp);
		i++;
	}
	if (ok == false)
		pipex->path_cmd_1 = NULL;
	else
		pipex->path_cmd_1 = ft_strjoin(pipex->env[i], tab[0]);
	free_tab(tab);
}

void	access_cmd_2(t_pipex *pipex)
{
	int		i = 0;
	bool	ok;
	char	**tab;
	char	*tmp;

	ok = false;
	tab = ft_split(pipex->av[3], ' ');
	while (pipex->env[i])
	{
		tmp = ft_strjoin(pipex->env[i], tab[0]);
		if (!access(tmp, X_OK))
		{
			free(tmp);
			ok = true;
			break ;
		}
		free(tmp);
		i++;
	}
	if (ok == false)
		pipex->path_cmd_2 = NULL;
	else
		pipex->path_cmd_2 = ft_strjoin(pipex->env[i], tab[0]);
	free_tab(tab);
}

void	create_file(t_pipex *pipex)
{
	pipex->infile_fd = open(pipex->av[1], O_RDONLY, 0644);
	if (pipex->infile_fd < 0)
	{
		ft_putstr_fd("bash: blabla: ", STDERR_FILENO);
		perror(NULL);
		// free_pipex(pipex);
		// exit(0);
	}
	pipex->outfile_fd = open(pipex->av[4], O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (pipex->outfile_fd < 0)
	{
		ft_putstr_fd("bash: blabla: ", STDERR_FILENO);
		perror(NULL);
		// free_pipex(pipex);
		// exit(0);
	}
}

void	child_process(t_pipex *pipex, int *fd, char **env)
{
	if (pipex->flag == 0 && pipex->infile_fd < 0)
		exit(1);
	if (pipex->flag == 1 && pipex->outfile_fd < 0)
		exit(1);
	if (pipex->flag == 0 && !pipex->path_cmd_1)
	{
		ft_putstr_fd(pipex->av[2], 2);
		ft_putstr_fd(": command not found\n", 2);
		exit(127);
	}
	if (pipex->flag == 1 && !pipex->path_cmd_2)
	{
		ft_putstr_fd(pipex->av[3], 2);
		ft_putstr_fd(": command not found\n", 2);
		exit(127);
	}
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
		free_pipex(pipex);
		exit(0);
	}
	else
	{
		execve(pipex->path_cmd_2, pipex->argv_cmd_2, env);
		free_pipex(pipex);
		exit(0);
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
	int		tmp = 0;
	pid_t	pid;

	pipe(fd);
	loop = 2;
	pipex->flag = 0;
	if (pipex->path_cmd_1)
	{
		pipex->argv_cmd_1 = ft_split(pipex->av[2], ' ');
		free(pipex->argv_cmd_1[0]);
		pipex->argv_cmd_1[0] = ft_strdup(pipex->path_cmd_1);	
	}
	if (pipex->path_cmd_2)
	{
		pipex->argv_cmd_2 = ft_split(pipex->av[3], ' ');
		free(pipex->argv_cmd_2[0]);
		pipex->argv_cmd_2[0] = ft_strdup(pipex->path_cmd_2);	
	}
	pipex->status = 0;
	while (loop > 0)
	{
		pid = fork();
		if (pid == 0)
			child_process(pipex, fd, env);
		else
			parent_process(pipex, fd);
		pipex->flag = 1;
		loop--;
		waitpid(pid, &tmp, 0);
		if (WIFEXITED(tmp))
			pipex->status = WEXITSTATUS(tmp);
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
		free_pipex(&pipex);
		return (pipex.status);
	}
	else
		return (0);

}
