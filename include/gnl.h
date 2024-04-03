/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gnl.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgriblin <tgriblin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/19 08:59:36 by tgriblin          #+#    #+#             */
/*   Updated: 2024/03/27 14:45:07 by tgriblin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GNL_H
# define GNL_H

# define BUFFER_SIZE 1000

# include <stdlib.h>
# include "minishell.h"

char	*get_next_line(int fd);
char	*ft_strjoin_bis(char *s1, char *s2);
void	*free_null(void *ptr1, void *ptr2);
char	*ft_strchr(const char *s, int c);

#endif