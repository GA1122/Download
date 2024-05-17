static void enable_swap_info(struct swap_info_struct *p, int prio,
				unsigned char *swap_map)
{
	int i, prev;

	spin_lock(&swap_lock);
	if (prio >= 0)
		p->prio = prio;
	else
		p->prio = --least_priority;
	p->swap_map = swap_map;
	p->flags |= SWP_WRITEOK;
	nr_swap_pages += p->pages;
	total_swap_pages += p->pages;

	 
	prev = -1;
	for (i = swap_list.head; i >= 0; i = swap_info[i]->next) {
		if (p->prio >= swap_info[i]->prio)
			break;
		prev = i;
	}
	p->next = i;
	if (prev < 0)
		swap_list.head = swap_list.next = p->type;
	else
		swap_info[prev]->next = p->type;
	spin_unlock(&swap_lock);
}
