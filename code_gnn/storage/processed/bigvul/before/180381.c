 check_compat_entry_size_and_hooks(struct compat_arpt_entry *e,
 				  struct xt_table_info *newinfo,
 				  unsigned int *size,
 				  const unsigned char *base,
 				  const unsigned char *limit,
 				  const unsigned int *hook_entries,
 				  const unsigned int *underflows,
 				  const char *name)
 {
 	struct xt_entry_target *t;
 	struct xt_target *target;
 	unsigned int entry_offset;
 	int ret, off, h;
  
  	duprintf("check_compat_entry_size_and_hooks %p\n", e);
  	if ((unsigned long)e % __alignof__(struct compat_arpt_entry) != 0 ||
	    (unsigned char *)e + sizeof(struct compat_arpt_entry) >= limit) {
// 	    (unsigned char *)e + sizeof(struct compat_arpt_entry) >= limit ||
// 	    (unsigned char *)e + e->next_offset > limit) {
  		duprintf("Bad offset %p, limit = %p\n", e, limit);
  		return -EINVAL;
  	}
 
 	if (e->next_offset < sizeof(struct compat_arpt_entry) +
 			     sizeof(struct compat_xt_entry_target)) {
 		duprintf("checking: element %p size %u\n",
 			 e, e->next_offset);
 		return -EINVAL;
 	}
 
 	 
 	ret = check_entry((struct arpt_entry *)e);
 	if (ret)
 		return ret;
 
 	off = sizeof(struct arpt_entry) - sizeof(struct compat_arpt_entry);
 	entry_offset = (void *)e - (void *)base;
 
 	t = compat_arpt_get_target(e);
 	target = xt_request_find_target(NFPROTO_ARP, t->u.user.name,
 					t->u.user.revision);
 	if (IS_ERR(target)) {
 		duprintf("check_compat_entry_size_and_hooks: `%s' not found\n",
 			 t->u.user.name);
 		ret = PTR_ERR(target);
 		goto out;
 	}
 	t->u.kernel.target = target;
 
 	off += xt_compat_target_offset(target);
 	*size += off;
 	ret = xt_compat_add_offset(NFPROTO_ARP, entry_offset, off);
 	if (ret)
 		goto release_target;
 
 	 
 	for (h = 0; h < NF_ARP_NUMHOOKS; h++) {
 		if ((unsigned char *)e - base == hook_entries[h])
 			newinfo->hook_entry[h] = hook_entries[h];
 		if ((unsigned char *)e - base == underflows[h])
 			newinfo->underflow[h] = underflows[h];
 	}
 
 	 
 	memset(&e->counters, 0, sizeof(e->counters));
 	e->comefrom = 0;
 	return 0;
 
 release_target:
 	module_put(t->u.kernel.target->me);
 out:
 	return ret;
 }