static int bgp_attr_atomic(struct bgp_attr_parser_args *args)
{
	struct attr *const attr = args->attr;
	const bgp_size_t length = args->length;

	 
	if (length != 0) {
		flog_err(EC_BGP_ATTR_LEN,
			 "ATOMIC_AGGREGATE attribute length isn't 0 [%u]",
			 length);
		return bgp_attr_malformed(args, BGP_NOTIFY_UPDATE_ATTR_LENG_ERR,
					  args->total);
	}

	 
	attr->flag |= ATTR_FLAG_BIT(BGP_ATTR_ATOMIC_AGGREGATE);

	return BGP_ATTR_PARSE_PROCEED;
}
