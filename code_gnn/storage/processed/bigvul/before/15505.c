static NOINLINE void attach_option(
		struct option_set **opt_list,
		const struct dhcp_optflag *optflag,
		char *buffer,
		int length)
{
	struct option_set *existing;
	char *allocated;

	allocated = allocate_tempopt_if_needed(optflag, buffer, &length);
#if ENABLE_FEATURE_UDHCP_RFC3397
	if ((optflag->flags & OPTION_TYPE_MASK) == OPTION_DNS_STRING) {
		 
		allocated = buffer = (char *)dname_enc(NULL, 0, buffer, &length);
	}
#endif

	existing = udhcp_find_option(*opt_list, optflag->code);
	if (!existing) {
		struct option_set *new, **curr;

		 
		log2("Attaching option %02x to list", optflag->code);
		new = xmalloc(sizeof(*new));
		new->data = xmalloc(length + OPT_DATA);
		new->data[OPT_CODE] = optflag->code;
		new->data[OPT_LEN] = length;
		memcpy(new->data + OPT_DATA, (allocated ? allocated : buffer), length);

		curr = opt_list;
		while (*curr && (*curr)->data[OPT_CODE] < optflag->code)
			curr = &(*curr)->next;

		new->next = *curr;
		*curr = new;
		goto ret;
	}

	if (optflag->flags & OPTION_LIST) {
		unsigned old_len;

		 
		log2("Attaching option %02x to existing member of list", optflag->code);
		old_len = existing->data[OPT_LEN];
		if (old_len + length < 255) {
			 

			existing->data = xrealloc(existing->data, OPT_DATA + 1 + old_len + length);
			if ((optflag->flags & OPTION_TYPE_MASK) == OPTION_STRING
			 || (optflag->flags & OPTION_TYPE_MASK) == OPTION_STRING_HOST
			) {
				 
				existing->data[OPT_DATA + old_len] = ' ';
				old_len++;
			}
			memcpy(existing->data + OPT_DATA + old_len, (allocated ? allocated : buffer), length);
			existing->data[OPT_LEN] = old_len + length;
		}  
	}  

 ret:
	free(allocated);
}