static bgp_attr_parse_ret_t bgp_attr_aspath_check(struct peer *const peer,
						  struct attr *const attr)
{
	 
	struct aspath *aspath;

	 
	if ((peer->sort == BGP_PEER_CONFED
	     && !aspath_left_confed_check(attr->aspath))
	    || (peer->sort == BGP_PEER_EBGP
		&& aspath_confed_check(attr->aspath))) {
		flog_err(EC_BGP_ATTR_MAL_AS_PATH, "Malformed AS path from %s",
			 peer->host);
		bgp_notify_send(peer, BGP_NOTIFY_UPDATE_ERR,
				BGP_NOTIFY_UPDATE_MAL_AS_PATH);
		return BGP_ATTR_PARSE_ERROR;
	}

	 
	if (CHECK_FLAG(peer->flags, PEER_FLAG_ENFORCE_FIRST_AS)) {
		if (peer->sort == BGP_PEER_EBGP
		    && !aspath_firstas_check(attr->aspath, peer->as)) {
			flog_err(EC_BGP_ATTR_FIRST_AS,
				 "%s incorrect first AS (must be %u)",
				 peer->host, peer->as);
			bgp_notify_send(peer, BGP_NOTIFY_UPDATE_ERR,
					BGP_NOTIFY_UPDATE_MAL_AS_PATH);
			return BGP_ATTR_PARSE_ERROR;
		}
	}

	 
	if (peer->change_local_as
	    && !CHECK_FLAG(peer->flags, PEER_FLAG_LOCAL_AS_NO_PREPEND)) {
		aspath = aspath_dup(attr->aspath);
		aspath = aspath_add_seq(aspath, peer->change_local_as);
		aspath_unintern(&attr->aspath);
		attr->aspath = aspath_intern(aspath);
	}

	return BGP_ATTR_PARSE_PROCEED;
}
