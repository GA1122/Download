ebt_get_udc_positions(struct ebt_entry *e, struct ebt_table_info *newinfo,
   unsigned int *n, struct ebt_cl_stack *udc)
{
	int i;

	 
	if (e->bitmask)
		return 0;
	for (i = 0; i < NF_BR_NUMHOOKS; i++) {
		if (newinfo->hook_entry[i] == (struct ebt_entries *)e)
			break;
	}
	 
	if (i != NF_BR_NUMHOOKS)
		return 0;

	udc[*n].cs.chaininfo = (struct ebt_entries *)e;
	 
	udc[*n].cs.n = 0;
	udc[*n].hookmask = 0;

	(*n)++;
	return 0;
}
