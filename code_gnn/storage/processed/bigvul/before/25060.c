static int dccp_v4_send_response(struct sock *sk, struct request_sock *req,
				 struct request_values *rv_unused)
{
	int err = -1;
	struct sk_buff *skb;
	struct dst_entry *dst;
	struct flowi4 fl4;

	dst = inet_csk_route_req(sk, &fl4, req);
	if (dst == NULL)
		goto out;

	skb = dccp_make_response(sk, dst, req);
	if (skb != NULL) {
		const struct inet_request_sock *ireq = inet_rsk(req);
		struct dccp_hdr *dh = dccp_hdr(skb);

		dh->dccph_checksum = dccp_v4_csum_finish(skb, ireq->loc_addr,
							      ireq->rmt_addr);
		err = ip_build_and_send_pkt(skb, sk, ireq->loc_addr,
					    ireq->rmt_addr,
					    ireq->opt);
		err = net_xmit_eval(err);
	}

out:
	dst_release(dst);
	return err;
}