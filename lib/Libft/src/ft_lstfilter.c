/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstfilter.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arosado- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 00:00:00 by arosado-          #+#    #+#             */
/*   Updated: 2026/01/07 00:00:00 by arosado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

/*
** Filters a list in-place based on a predicate function.
** Elements that don't pass the predicate are deleted using the del function.
**
** Parameters:
**   lst: pointer to the first element of the list
**   predicate: function that returns 1 (keep) or 0 (delete)
**   del: function to delete the content of an element
*/
void	ft_lstfilter(t_list **lst, int (*predicate)(void *), void (*del)(void *))
{
	t_list	*current;
	t_list	*next;
	t_list	**prev_ptr;

	if (!lst || !predicate || !del)
		return ;
	prev_ptr = lst;
	current = *lst;
	while (current)
	{
		next = current->next;
		if (!predicate(current->content))
		{
			del(current->content);
			free(current);
			*prev_ptr = next;
		}
		else
			prev_ptr = &current->next;
		current = next;
	}
}
