mark_source_chains(const struct xt_table_info *newinfo,
		   unsigned int valid_hooks, void *entry0)
{
	unsigned int hook;

	 
	for (hook = 0; hook < NF_INET_NUMHOOKS; hook++) {
		unsigned int pos = newinfo->hook_entry[hook];
		struct ip6t_entry *e = (struct ip6t_entry *)(entry0 + pos);

		if (!(valid_hooks & (1 << hook)))
			continue;

		 
		e->counters.pcnt = pos;

		for (;;) {
			const struct xt_standard_target *t
				= (void *)ip6t_get_target_c(e);
			int visited = e->comefrom & (1 << hook);

			if (e->comefrom & (1 << NF_INET_NUMHOOKS)) {
				pr_err("iptables: loop hook %u pos %u %08X.\n",
				       hook, pos, e->comefrom);
				return 0;
			}
			e->comefrom |= ((1 << hook) | (1 << NF_INET_NUMHOOKS));

			 
			if ((unconditional(e) &&
			     (strcmp(t->target.u.user.name,
				     XT_STANDARD_TARGET) == 0) &&
			     t->verdict < 0) || visited) {
				unsigned int oldpos, size;

				if ((strcmp(t->target.u.user.name,
					    XT_STANDARD_TARGET) == 0) &&
				    t->verdict < -NF_MAX_VERDICT - 1) {
					duprintf("mark_source_chains: bad "
						"negative verdict (%i)\n",
								t->verdict);
					return 0;
				}

				 
				do {
					e->comefrom ^= (1<<NF_INET_NUMHOOKS);
#ifdef DEBUG_IP_FIREWALL_USER
					if (e->comefrom
					    & (1 << NF_INET_NUMHOOKS)) {
						duprintf("Back unset "
							 "on hook %u "
							 "rule %u\n",
							 hook, pos);
					}
#endif
					oldpos = pos;
					pos = e->counters.pcnt;
					e->counters.pcnt = 0;

					 
					if (pos == oldpos)
						goto next;

					e = (struct ip6t_entry *)
						(entry0 + pos);
				} while (oldpos == pos + e->next_offset);

				 
				size = e->next_offset;
				e = (struct ip6t_entry *)
					(entry0 + pos + size);
				if (pos + size >= newinfo->size)
					return 0;
				e->counters.pcnt = pos;
				pos += size;
			} else {
				int newpos = t->verdict;

				if (strcmp(t->target.u.user.name,
					   XT_STANDARD_TARGET) == 0 &&
				    newpos >= 0) {
					if (newpos > newinfo->size -
						sizeof(struct ip6t_entry)) {
						duprintf("mark_source_chains: "
							"bad verdict (%i)\n",
								newpos);
						return 0;
					}
					 
					duprintf("Jump rule %u -> %u\n",
						 pos, newpos);
					e = (struct ip6t_entry *)
						(entry0 + newpos);
					if (!find_jump_target(newinfo, e))
						return 0;
				} else {
					 
					newpos = pos + e->next_offset;
					if (newpos >= newinfo->size)
						return 0;
				}
				e = (struct ip6t_entry *)
					(entry0 + newpos);
				e->counters.pcnt = pos;
				pos = newpos;
			}
		}
next:
		duprintf("Finished chain %u\n", hook);
	}
	return 1;
}