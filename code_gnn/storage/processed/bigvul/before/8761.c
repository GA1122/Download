int FAST_FUNC udhcp_str2optset(const char *const_str, void *arg,
		const struct dhcp_optflag *optflags, const char *option_strings,
		bool dhcpv6)
{
	struct option_set **opt_list = arg;
	char *opt;
	char *str;
	const struct dhcp_optflag *optflag;
	struct dhcp_optflag userdef_optflag;
	unsigned optcode;
	int retval;
	 
	char buffer[9] ALIGNED(4);
	uint16_t *result_u16 = (uint16_t *) buffer;
	uint32_t *result_u32 = (uint32_t *) buffer;

	 
	str = (char *) const_str;
	opt = strtok(str, " \t=:");
	if (!opt)
		return 0;

	optcode = bb_strtou(opt, NULL, 0);
	if (!errno && optcode < 255) {
		 
		userdef_optflag.flags = OPTION_BIN;
		userdef_optflag.code = optcode;
		optflag = &userdef_optflag;
	} else {
		optflag = &optflags[udhcp_option_idx(opt, option_strings)];
	}

	 
	retval = 0;
	do {
		int length;
		char *val;

		if (optflag->flags == OPTION_BIN) {
			val = strtok(NULL, "");  
			trim(val);
		} else
			val = strtok(NULL, ", \t");
		if (!val)
			break;

		length = dhcp_option_lengths[optflag->flags & OPTION_TYPE_MASK];
		retval = 0;
		opt = buffer;  

		switch (optflag->flags & OPTION_TYPE_MASK) {
		case OPTION_IP:
			retval = udhcp_str2nip(val, buffer);
			break;
		case OPTION_IP_PAIR:
			retval = udhcp_str2nip(val, buffer);
			val = strtok(NULL, ", \t/-");
			if (!val)
				retval = 0;
			if (retval)
				retval = udhcp_str2nip(val, buffer + 4);
			break;
 case_OPTION_STRING:
		case OPTION_STRING:
		case OPTION_STRING_HOST:
#if ENABLE_FEATURE_UDHCP_RFC3397
		case OPTION_DNS_STRING:
#endif
			length = strnlen(val, 254);
			if (length > 0) {
				opt = val;
				retval = 1;
			}
			break;
		case OPTION_U8:
			buffer[0] = bb_strtou32(val, NULL, 0);
			retval = (errno == 0);
			break;
		 
		 
		case OPTION_U16: {
			uint32_t tmp = bb_strtou32(val, NULL, 0);
			*result_u16 = htons(tmp);
			retval = (errno == 0  );
			break;
		}
		case OPTION_U32: {
			uint32_t tmp = bb_strtou32(val, NULL, 0);
			*result_u32 = htonl(tmp);
			retval = (errno == 0);
			break;
		}
		case OPTION_S32: {
			int32_t tmp = bb_strtoi32(val, NULL, 0);
			*result_u32 = htonl(tmp);
			retval = (errno == 0);
			break;
		}
		case OPTION_STATIC_ROUTES: {
			 
			 
			unsigned mask;
			char *slash = strchr(val, '/');
			if (slash) {
				*slash = '\0';
				retval = udhcp_str2nip(val, buffer + 1);
				buffer[0] = mask = bb_strtou(slash + 1, NULL, 10);
				val = strtok(NULL, ", \t/-");
				if (!val || mask > 32 || errno)
					retval = 0;
				if (retval) {
					length = ((mask + 7) >> 3) + 5;
					retval = udhcp_str2nip(val, buffer + (length - 4));
				}
			}
			break;
		}
		case OPTION_BIN:
			 
			if (val[0] == '"' || val[0] == '\'') {
				char delim = val[0];
				char *end = last_char_is(val + 1, delim);
				if (end) {
					*end = '\0';
					val++;
					userdef_optflag.flags = OPTION_STRING;
					goto case_OPTION_STRING;
				}
			}
			 
			opt = val;
			retval = 1;
			break;
		default:
			break;
		}

		if (retval)
			attach_option(opt_list, optflag, opt, length, dhcpv6);
	} while (retval && (optflag->flags & OPTION_LIST));

	return retval;
}
