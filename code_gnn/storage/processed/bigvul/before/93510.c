static void ip6_mr_forward(struct net *net, struct mr6_table *mrt,
			   struct sk_buff *skb, struct mfc6_cache *cache)
{
	int psend = -1;
	int vif, ct;
	int true_vifi = ip6mr_find_vif(mrt, skb->dev);

	vif = cache->mf6c_parent;
	cache->mfc_un.res.pkt++;
	cache->mfc_un.res.bytes += skb->len;
	cache->mfc_un.res.lastuse = jiffies;

	if (ipv6_addr_any(&cache->mf6c_origin) && true_vifi >= 0) {
		struct mfc6_cache *cache_proxy;

		 
		cache_proxy = ip6mr_cache_find_any_parent(mrt, vif);
		if (cache_proxy &&
		    cache_proxy->mfc_un.res.ttls[true_vifi] < 255)
			goto forward;
	}

	 
	if (mrt->vif6_table[vif].dev != skb->dev) {
		cache->mfc_un.res.wrong_if++;

		if (true_vifi >= 0 && mrt->mroute_do_assert &&
		     
		    (mrt->mroute_do_pim ||
		     cache->mfc_un.res.ttls[true_vifi] < 255) &&
		    time_after(jiffies,
			       cache->mfc_un.res.last_assert + MFC_ASSERT_THRESH)) {
			cache->mfc_un.res.last_assert = jiffies;
			ip6mr_cache_report(mrt, skb, true_vifi, MRT6MSG_WRONGMIF);
		}
		goto dont_forward;
	}

forward:
	mrt->vif6_table[vif].pkt_in++;
	mrt->vif6_table[vif].bytes_in += skb->len;

	 
	if (ipv6_addr_any(&cache->mf6c_origin) &&
	    ipv6_addr_any(&cache->mf6c_mcastgrp)) {
		if (true_vifi >= 0 &&
		    true_vifi != cache->mf6c_parent &&
		    ipv6_hdr(skb)->hop_limit >
				cache->mfc_un.res.ttls[cache->mf6c_parent]) {
			 
			psend = cache->mf6c_parent;
			goto last_forward;
		}
		goto dont_forward;
	}
	for (ct = cache->mfc_un.res.maxvif - 1; ct >= cache->mfc_un.res.minvif; ct--) {
		 
		if ((!ipv6_addr_any(&cache->mf6c_origin) || ct != true_vifi) &&
		    ipv6_hdr(skb)->hop_limit > cache->mfc_un.res.ttls[ct]) {
			if (psend != -1) {
				struct sk_buff *skb2 = skb_clone(skb, GFP_ATOMIC);
				if (skb2)
					ip6mr_forward2(net, mrt, skb2, cache, psend);
			}
			psend = ct;
		}
	}
last_forward:
	if (psend != -1) {
		ip6mr_forward2(net, mrt, skb, cache, psend);
		return;
	}

dont_forward:
	kfree_skb(skb);
}