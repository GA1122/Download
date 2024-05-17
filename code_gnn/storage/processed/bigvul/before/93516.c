static struct mfc6_cache *ip6mr_cache_find_any(struct mr6_table *mrt,
					       struct in6_addr *mcastgrp,
					       mifi_t mifi)
{
	int line = MFC6_HASH(mcastgrp, &in6addr_any);
	struct mfc6_cache *c, *proxy;

	if (ipv6_addr_any(mcastgrp))
		goto skip;

	list_for_each_entry(c, &mrt->mfc6_cache_array[line], list)
		if (ipv6_addr_any(&c->mf6c_origin) &&
		    ipv6_addr_equal(&c->mf6c_mcastgrp, mcastgrp)) {
			if (c->mfc_un.res.ttls[mifi] < 255)
				return c;

			 
			proxy = ip6mr_cache_find_any_parent(mrt,
							    c->mf6c_parent);
			if (proxy && proxy->mfc_un.res.ttls[mifi] < 255)
				return c;
		}

skip:
	return ip6mr_cache_find_any_parent(mrt, mifi);
}