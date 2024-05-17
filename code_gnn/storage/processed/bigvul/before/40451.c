static int ipxitf_demux_socket(struct ipx_interface *intrfc,
			       struct sk_buff *skb, int copy)
{
	struct ipxhdr *ipx = ipx_hdr(skb);
	struct sock *sock1 = NULL, *sock2 = NULL;
	struct sk_buff *skb1 = NULL, *skb2 = NULL;
	int rc;

	if (intrfc == ipx_primary_net && ntohs(ipx->ipx_dest.sock) == 0x451)
		sock1 = ncp_connection_hack(intrfc, ipx);
	if (!sock1)
		 
		sock1 = ipxitf_find_socket(intrfc, ipx->ipx_dest.sock);

	 

	if (ipx_primary_net && intrfc != ipx_primary_net) {
		const int dsock = ntohs(ipx->ipx_dest.sock);

		if (dsock == 0x452 || dsock == 0x453 || dsock == 0x456)
			 
			sock2 = ipxitf_find_socket(ipx_primary_net,
							ipx->ipx_dest.sock);
	}

	 
	rc = 0;
	if (!sock1 && !sock2) {
		if (!copy)
			kfree_skb(skb);
		goto out;
	}

	 

	if (copy)
		skb1 = skb_clone(skb, GFP_ATOMIC);
	else
		skb1 = skb;

	rc = -ENOMEM;
	if (!skb1)
		goto out_put;

	 
	if (sock1 && sock2)
		skb2 = skb_clone(skb1, GFP_ATOMIC);
	else
		skb2 = skb1;

	if (sock1)
		ipxitf_def_skb_handler(sock1, skb1);

	if (!skb2)
		goto out_put;

	if (sock2)
		ipxitf_def_skb_handler(sock2, skb2);

	rc = 0;
out_put:
	if (sock1)
		sock_put(sock1);
	if (sock2)
		sock_put(sock2);
out:
	return rc;
}
