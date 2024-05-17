static int mark_source_chains(const struct xt_table_info *newinfo,
			      unsigned int valid_hooks, void *entry0,
			      unsigned int *offsets)
{
	unsigned int hook;

	 
	for (hook = 0; hook < NF_ARP_NUMHOOKS; hook++) {
		unsigned int pos = newinfo->hook_entry[hook];
		struct arpt_entry *e = entry0 + pos;

		if (!(valid_hooks & (1 << hook)))
			continue;

		 
		e->counters.pcnt = pos;

		for (;;) {
			const struct xt_standard_target *t
				= (void *)arpt_get_target_c(e);
			int visited = e->comefrom & (1 << hook);

			if (e->comefrom & (1 << NF_ARP_NUMHOOKS))
				return 0;

			e->comefrom
				|= ((1 << hook) | (1 << NF_ARP_NUMHOOKS));

			 
			if ((unconditional(e) &&
			     (strcmp(t->target.u.user.name,
				     XT_STANDARD_TARGET) == 0) &&
			     t->verdict < 0) || visited) {
				unsigned int oldpos, size;

				if ((strcmp(t->target.u.user.name,
					    XT_STANDARD_TARGET) == 0) &&
				    t->verdict < -NF_MAX_VERDICT - 1)
					return 0;

				 
				do {
					e->comefrom ^= (1<<NF_ARP_NUMHOOKS);
					oldpos = pos;
					pos = e->counters.pcnt;
					e->counters.pcnt = 0;

					 
					if (pos == oldpos)
						goto next;

					e = entry0 + pos;
				} while (oldpos == pos + e->next_offset);

				 
				size = e->next_offset;
				e = entry0 + pos + size;
				if (pos + size >= newinfo->size)
					return 0;
				e->counters.pcnt = pos;
				pos += size;
			} else {
				int newpos = t->verdict;

				if (strcmp(t->target.u.user.name,
					   XT_STANDARD_TARGET) == 0 &&
				    newpos >= 0) {
					 
					if (!xt_find_jump_offset(offsets, newpos,
								 newinfo->number))
						return 0;
				} else {
					 
					newpos = pos + e->next_offset;
					if (newpos >= newinfo->size)
						return 0;
				}
				e = entry0 + newpos;
				e->counters.pcnt = pos;
				pos = newpos;
			}
		}
next:		;
	}
	return 1;
}