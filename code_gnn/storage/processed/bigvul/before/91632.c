static int bgp_attr_flag_invalid(struct bgp_attr_parser_args *args)
{
	uint8_t mask = BGP_ATTR_FLAG_EXTLEN;
	const uint8_t flags = args->flags;
	const uint8_t attr_code = args->type;

	 
	if (attr_code > attr_flags_values_max)
		return 0;
	if (attr_flags_values[attr_code] == 0)
		return 0;

	 
	if (!CHECK_FLAG(BGP_ATTR_FLAG_OPTIONAL, flags)
	    && !CHECK_FLAG(BGP_ATTR_FLAG_TRANS, flags)) {
		flog_err(
			EC_BGP_ATTR_FLAG,
			"%s well-known attributes must have transitive flag set (%x)",
			lookup_msg(attr_str, attr_code, NULL), flags);
		return 1;
	}

	 
	if (CHECK_FLAG(flags, BGP_ATTR_FLAG_PARTIAL)) {
		if (!CHECK_FLAG(flags, BGP_ATTR_FLAG_OPTIONAL)) {
			flog_err(EC_BGP_ATTR_FLAG,
				 "%s well-known attribute "
				 "must NOT have the partial flag set (%x)",
				 lookup_msg(attr_str, attr_code, NULL), flags);
			return 1;
		}
		if (CHECK_FLAG(flags, BGP_ATTR_FLAG_OPTIONAL)
		    && !CHECK_FLAG(flags, BGP_ATTR_FLAG_TRANS)) {
			flog_err(EC_BGP_ATTR_FLAG,
				 "%s optional + transitive attribute "
				 "must NOT have the partial flag set (%x)",
				 lookup_msg(attr_str, attr_code, NULL), flags);
			return 1;
		}
	}

	 
	if (CHECK_FLAG(flags, BGP_ATTR_FLAG_OPTIONAL)
	    && CHECK_FLAG(flags, BGP_ATTR_FLAG_TRANS))
		SET_FLAG(mask, BGP_ATTR_FLAG_PARTIAL);

	if ((flags & ~mask) == attr_flags_values[attr_code])
		return 0;

	bgp_attr_flags_diagnose(args, attr_flags_values[attr_code]);
	return 1;
}
