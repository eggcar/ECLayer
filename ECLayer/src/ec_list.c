/**
  *@file	ec_list.c
  *@brief	Basic loop list operations. Be careful, this is not the Linux list.h.
  *@author	Eggcar
  */

#include "ec_list.h"
#include <stddef.h>
#include <stdint.h>

void list_sort(list_t* head, cmp_list_node_f cmp)
{
	list_t list_buf;
	list_t* p_list_buf = &list_buf;
	if (list_is_empty(head)) {
		return;
	}
	if (list_is_singular(head)) {
		return;
	}
	list_clear(p_list_buf);
	list_split(head, p_list_buf, head->next);
	for(list_t* itr_unsorted = p_list_buf->next; 
		itr_unsorted != p_list_buf; 
		itr_unsorted = p_list_buf->next) {
		foreach(itr_sorted, head) {
			if (cmp(itr_unsorted, itr_sorted) < 0) {
				list_delete(itr_unsorted);
				list_insert(itr_unsorted, itr_sorted->prev, itr_sorted);
				goto next_unsorted;
			}
			else if (list_is_last(head, itr_sorted)) {
				list_delete(itr_unsorted);
				list_insert(itr_unsorted, itr_sorted, itr_sorted->next);
				goto next_unsorted;
			}
			else {
				
			}
		}
next_unsorted :
		continue;
	}
}
