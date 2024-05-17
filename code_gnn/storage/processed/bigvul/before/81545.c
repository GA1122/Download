update_eval_map(union trace_eval_map_item *ptr)
{
	if (!ptr->map.eval_string) {
		if (ptr->tail.next) {
			ptr = ptr->tail.next;
			 
			ptr++;
		} else
			return NULL;
	}
	return ptr;
}
