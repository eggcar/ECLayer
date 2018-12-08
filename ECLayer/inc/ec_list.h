/**
 * @file	ec_list.h
 * @brief	Basic loop list operations. Be careful, this is not the Linux list.h.
 * @author	Eggcar
*/

/**
 * Copyright EggCar(eggcar@qq.com)
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * 	http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/

#ifndef __EC_LIST_H
#define __EC_LIST_H

#include <stddef.h>
#include <stdint.h>

#define container_of(ptr, type, member) ({			\
	const typeof( ((type *)0)->member ) *__mptr = (ptr);	\
	(type *)( (char *)__mptr - offsetof(type,member) ); })

#define list_get_node(list_ptr, node_type, list_name) \
	container_of(list_ptr, node_type, list_name)

#define list_head_def(name) \
	list_t name = {.prev = &(name), .next = &(name)}

typedef struct list_struct {
	struct list_struct *prev;
	struct list_struct *next;
} list_t;

/**
  *@brief	Compare two list node with customized function.
  *@param	list_t* a	the first list node
  *@param	list_t* b	the second list node
  *@retval	if a > b return positive, if a = b return 0, if a < b return negtive
  */
typedef int (*cmp_list_node_f)(list_t *a, list_t *b);

#define list_is_last(list_head, node) \
	((node->next == list_head) && (list_head->prev == node))

#define list_not_last(list_head, node) \
	(!list_is_last(list_head, node))

#define list_is_empty(list_head) \
	((list_head->prev == list_head) && (list_head->next == list_head))

#define list_is_singular(list_head) \
	((list_head->next->next == list_head) && (!list_is_empty(list_head)))

#define foreach(iterator, list_head) \
	for (list_t *iterator = (list_head)->next; iterator != (list_head); iterator = iterator->next)

static inline void list_insert(list_t *new_node, list_t *prev_node, list_t *next_node)
{
	prev_node->next = new_node;
	new_node->prev = prev_node;
	new_node->next = next_node;
	next_node->prev = new_node;
}

static inline void list_delete(list_t *node)
{
	node->prev->next = node->next;
	node->next->prev = node->prev;
	node->prev = node;
	node->next = node;
}

static inline void list_swap(list_t *a, list_t *b)
{
	list_t tmp;
	tmp.prev = a->prev;
	tmp.next = a->next;
	a->prev->next = b;
	a->next->prev = b;
	b->prev->next = a;
	b->next->prev = a;
	a->prev = b->prev;
	a->next = b->next;
	b->prev = tmp.prev;
	b->next = tmp.next;
}

static inline void list_append(list_t *new_node, list_t *head)
{
	list_insert(new_node, head->prev, head);
}

static inline void list_clear(list_t *head)
{
	head->prev = head;
	head->next = head;
}

static inline list_t *list_lastof(list_t *head)
{
	return head->prev;
}

static inline void list_split(list_t *head, list_t *new_list, list_t *brk_point)
{
	if (list_is_empty(head)) {
		return;
	}
	if (brk_point == head) {
		new_list->next = head->next;
		new_list->prev = head->prev;
		head->next->prev = new_list;
		head->next->next = new_list;
		list_clear(head);
		return;
	}
	if (list_is_singular(head)) {
		return;
	}
	new_list->prev = list_lastof(head);
	new_list->prev->next = new_list;
	new_list->next = brk_point->next;
	new_list->next->prev = new_list;
	brk_point->next = head;
	head->prev = brk_point;
}

static inline list_t *list_index(list_t *head, uint32_t index)
{
	list_t *ptr = head->next;
	for (int i = 0; i < index; i++) {
		ptr = ptr->next;
	}
	return ptr;
}

static inline uint32_t list_index_of(list_t *head, list_t *node)
{
	uint32_t i = 0;
	if (list_is_empty(head)) {
		return 0;
	}
	foreach (iterator, head) {
		if (iterator == node) {
			return i;
		}
		i++;
	}
	return UINT32_MAX;
}

static inline uint32_t list_is_in(list_t *head, list_t *node)
{
	if (list_is_empty(head)) {
		if (head == node) {
			return 1;
		}
		else {
			return 0;
		}
	}
	foreach (iterator, head) {
		if (node == iterator) {
			return 1;
		}
	}
	return 0;
}

static inline list_t *list_max(list_t *head, cmp_list_node_f cmp)
{
	list_t *max = head->next;
	if (list_is_empty(head)) {
		return head;
	}
	foreach (itr_node, head) {
		if (cmp(max, itr_node) < 0) {
			max = itr_node;
		}
	}
	return max;
}

static inline list_t *list_min(list_t *head, cmp_list_node_f cmp)
{
	list_t *min = head->next;
	if (list_is_empty(head)) {
		return head;
	}
	foreach (itr_node, head) {
		if (cmp(min, itr_node) > 0) {
			min = itr_node;
		}
	}
	return min;
}

void list_sort(list_t *head, cmp_list_node_f cmp);

#endif
