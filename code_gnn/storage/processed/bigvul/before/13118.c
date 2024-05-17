static NOINLINE void attach_option(
		struct option_set **opt_list,
		const struct dhcp_optflag *optflag,
		char *buffer,
		int length,
		bool dhcpv6)
{
	IF_NOT_UDHCPC6(bool dhcpv6 = 0;)
	struct option_set *existing;
	char *allocated = NULL;

	if ((optflag->flags & OPTION_TYPE_MASK) == OPTION_BIN) {
		const char *end;
		allocated = xstrdup(buffer);  
		end = hex2bin(allocated, buffer, 255);
		if (errno)
			bb_error_msg_and_die("malformed hex string '%s'", buffer);
		length = end - allocated;
	}
#if ENABLE_FEATURE_UDHCP_RFC3397
	if ((optflag->flags & OPTION_TYPE_MASK) == OPTION_DNS_STRING) {
		 
		allocated = buffer = (char *)dname_enc(NULL, 0, buffer, &length);
	}
#endif

	existing = udhcp_find_option(*opt_list, optflag->code);
	if (!existing) {
		struct option_set *new, **curr;

		 
		log2("attaching option %02x to list", optflag->code);
		new = xmalloc(sizeof(*new));
		if (!dhcpv6) {
			new->data = xmalloc(length + OPT_DATA);
			new->data[OPT_CODE] = optflag->code;
			new->data[OPT_LEN] = length;
			memcpy(new->data + OPT_DATA, (allocated ? allocated : buffer),
					length);
		} else {
			new->data = xmalloc(length + D6_OPT_DATA);
			new->data[D6_OPT_CODE] = optflag->code >> 8;
			new->data[D6_OPT_CODE + 1] = optflag->code & 0xff;
			new->data[D6_OPT_LEN] = length >> 8;
			new->data[D6_OPT_LEN + 1] = length & 0xff;
			memcpy(new->data + D6_OPT_DATA, (allocated ? allocated : buffer),
					length);
		}

		curr = opt_list;
		while (*curr && (*curr)->data[OPT_CODE] < optflag->code)
			curr = &(*curr)->next;

		new->next = *curr;
		*curr = new;
		goto ret;
	}

	if (optflag->flags & OPTION_LIST) {
		unsigned old_len;

		 
		log2("attaching option %02x to existing member of list", optflag->code);
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
