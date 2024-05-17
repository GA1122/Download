static int translate_table(struct net *net, const char *name,
			   struct ebt_table_info *newinfo)
{
	unsigned int i, j, k, udc_cnt;
	int ret;
	struct ebt_cl_stack *cl_s = NULL;  

	i = 0;
	while (i < NF_BR_NUMHOOKS && !newinfo->hook_entry[i])
		i++;
	if (i == NF_BR_NUMHOOKS) {
		BUGPRINT("No valid hooks specified\n");
		return -EINVAL;
	}
	if (newinfo->hook_entry[i] != (struct ebt_entries *)newinfo->entries) {
		BUGPRINT("Chains don't start at beginning\n");
		return -EINVAL;
	}
	 
	for (j = i + 1; j < NF_BR_NUMHOOKS; j++) {
		if (!newinfo->hook_entry[j])
			continue;
		if (newinfo->hook_entry[j] <= newinfo->hook_entry[i]) {
			BUGPRINT("Hook order must be followed\n");
			return -EINVAL;
		}
		i = j;
	}

	 
	i = 0;  
	j = 0;  
	k = 0;  
	udc_cnt = 0;  
	ret = EBT_ENTRY_ITERATE(newinfo->entries, newinfo->entries_size,
	   ebt_check_entry_size_and_hooks, newinfo,
	   &i, &j, &k, &udc_cnt);

	if (ret != 0)
		return ret;

	if (i != j) {
		BUGPRINT("nentries does not equal the nr of entries in the "
			 "(last) chain\n");
		return -EINVAL;
	}
	if (k != newinfo->nentries) {
		BUGPRINT("Total nentries is wrong\n");
		return -EINVAL;
	}

	 
	if (udc_cnt) {
		 
		newinfo->chainstack =
			vmalloc(nr_cpu_ids * sizeof(*(newinfo->chainstack)));
		if (!newinfo->chainstack)
			return -ENOMEM;
		for_each_possible_cpu(i) {
			newinfo->chainstack[i] =
			  vmalloc(udc_cnt * sizeof(*(newinfo->chainstack[0])));
			if (!newinfo->chainstack[i]) {
				while (i)
					vfree(newinfo->chainstack[--i]);
				vfree(newinfo->chainstack);
				newinfo->chainstack = NULL;
				return -ENOMEM;
			}
		}

		cl_s = vmalloc(udc_cnt * sizeof(*cl_s));
		if (!cl_s)
			return -ENOMEM;
		i = 0;  
		EBT_ENTRY_ITERATE(newinfo->entries, newinfo->entries_size,
		   ebt_get_udc_positions, newinfo, &i, cl_s);
		 
		if (i != udc_cnt) {
			BUGPRINT("i != udc_cnt\n");
			vfree(cl_s);
			return -EFAULT;
		}
	}

	 
	for (i = 0; i < NF_BR_NUMHOOKS; i++)
		if (newinfo->hook_entry[i])
			if (check_chainloops(newinfo->hook_entry[i],
			   cl_s, udc_cnt, i, newinfo->entries)) {
				vfree(cl_s);
				return -EINVAL;
			}

	 

	 
	i = 0;
	ret = EBT_ENTRY_ITERATE(newinfo->entries, newinfo->entries_size,
	   ebt_check_entry, net, newinfo, name, &i, cl_s, udc_cnt);
	if (ret != 0) {
		EBT_ENTRY_ITERATE(newinfo->entries, newinfo->entries_size,
				  ebt_cleanup_entry, net, &i);
	}
	vfree(cl_s);
	return ret;
}
