static int ipxitf_rcv(struct ipx_interface *intrfc, struct sk_buff *skb)
{
	struct ipxhdr *ipx = ipx_hdr(skb);
	int rc = 0;

	ipxitf_hold(intrfc);

	 
	if (!intrfc->if_netnum)  
		ipxitf_discover_netnum(intrfc, skb);

	IPX_SKB_CB(skb)->last_hop.index = -1;
	if (ipx->ipx_type == IPX_TYPE_PPROP) {
		rc = ipxitf_pprop(intrfc, skb);
		if (rc)
			goto out_free_skb;
	}

	 
	if (!IPX_SKB_CB(skb)->ipx_dest_net)
		IPX_SKB_CB(skb)->ipx_dest_net = intrfc->if_netnum;
	if (!IPX_SKB_CB(skb)->ipx_source_net)
		IPX_SKB_CB(skb)->ipx_source_net = intrfc->if_netnum;

	 
	if (ipx->ipx_type != IPX_TYPE_PPROP &&
	    intrfc->if_netnum != IPX_SKB_CB(skb)->ipx_dest_net) {
		 
		if (skb->pkt_type == PACKET_HOST) {
			skb = skb_unshare(skb, GFP_ATOMIC);
			if (skb)
				rc = ipxrtr_route_skb(skb);
			goto out_intrfc;
		}

		goto out_free_skb;
	}

	 
	if (!memcmp(ipx_broadcast_node, ipx->ipx_dest.node, IPX_NODE_LEN) ||
	    !memcmp(intrfc->if_node, ipx->ipx_dest.node, IPX_NODE_LEN)) {
		rc = ipxitf_demux_socket(intrfc, skb, 0);
		goto out_intrfc;
	}

	 
out_free_skb:
	kfree_skb(skb);
out_intrfc:
	ipxitf_put(intrfc);
	return rc;
}
