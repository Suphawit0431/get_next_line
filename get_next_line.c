/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssaensuk <ssaensuk@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 21:44:38 by ssaensuk          #+#    #+#             */
/*   Updated: 2025/11/23 17:54:48 by ssaensuk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*ft_fill_stash(int fd, char *stash, char *buffer)
{
	ssize_t	byte_read;
	char	*tmp;
	char	*nl_ptr;

	byte_read = 1;
	nl_ptr = ft_strchr(stash, '\n');
	while (nl_ptr == NULL && byte_read > 0)
	{
		byte_read = read(fd, buffer, BUFFER_SIZE);
		if (byte_read <= 0)
			break ;
		buffer[byte_read] = '\0';
		tmp = stash;
		stash = ft_strjoin(tmp, buffer);
		free(tmp);
		nl_ptr = ft_strchr(stash, '\n');
	}
	if (byte_read == -1 || (byte_read <= 0 && stash[0] == '\0'))
	{
		free(stash);
		stash = NULL;
		return (NULL);
	}
	return (stash);
}

void	ft_free_stash(char **stash, int i)
{
	char	*tmp;

	tmp = *stash;
	*stash = ft_strdup(*stash + i + 1);
	free(tmp);
}

static char	*ft_get_line(char **stash)
{
	char	*line;
	int		i;

	i = 0;
	while ((*stash)[i] != '\n' && (*stash)[i] != '\0')
		i++;
	if ((*stash)[i] == '\n')
	{
		line = ft_substr(*stash, 0, i + 1);
		ft_free_stash(stash, i);
		if ((*stash)[0] == '\0')
		{
			free(*stash);
			*stash = NULL;
		}
	}
	else
	{
		line = ft_strdup(*stash);
		free(*stash);
		*stash = NULL;
	}
	return (line);
}

char	*get_next_line(int fd)
{
	static char	*stash;
	char		*buffer;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buffer = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
		return (NULL);
	if (!stash)
		stash = ft_strdup("");
	stash = ft_fill_stash(fd, stash, buffer);
	free(buffer);
	if (!stash)
		return (NULL);
	line = ft_get_line(&stash);
	if (!line)
	{
		free(stash);
		stash = NULL;
	}
	return (line);
}
