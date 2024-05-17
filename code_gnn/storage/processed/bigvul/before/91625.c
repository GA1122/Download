bgp_attr_cluster_list(struct bgp_attr_parser_args *args)
{
	struct peer *const peer = args->peer;
	struct attr *const attr = args->attr;
	const bgp_size_t length = args->length;

	 
	if (length % 4) {
		flog_err(EC_BGP_ATTR_LEN, "Bad cluster list length %d", length);

		return bgp_attr_malformed(args, BGP_NOTIFY_UPDATE_ATTR_LENG_ERR,
					  args->total);
	}

	attr->cluster =
		cluster_parse((struct in_addr *)stream_pnt(peer->curr), length);

	 
	stream_forward_getp(peer->curr, length);

	attr->flag |= ATTR_FLAG_BIT(BGP_ATTR_CLUSTER_LIST);

	return BGP_ATTR_PARSE_PROCEED;
}