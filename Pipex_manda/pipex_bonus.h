/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skock <skock@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 14:13:58 by skock             #+#    #+#             */
/*   Updated: 2025/02/03 14:39:05 by skock            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

# include "library/libft/libft.h"
# include <sys/wait.h>
# include <stdbool.h>
# include <errno.h>

typedef struct s_pipex
{
	char	**env;
	char	**av;
	char	**argv;
	char	*path_cmd_1;
	char	*path_cmd_2;
	int		ac;
	int		flag;
	int		index;
	int		status;
	int		infile_fd;
	int		outfile_fd;
}			t_pipex;

#endif