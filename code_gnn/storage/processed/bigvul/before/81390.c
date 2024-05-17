trace_eval_jmp_to_tail(union trace_eval_map_item *ptr)
{
	 
	return ptr + ptr->head.length + 1;
}
