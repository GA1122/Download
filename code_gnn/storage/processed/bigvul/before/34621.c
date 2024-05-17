int sctp_backlog_rcv(struct sock *sk, struct sk_buff *skb)
{
	struct sctp_chunk *chunk = SCTP_INPUT_CB(skb)->chunk;
	struct sctp_inq *inqueue = &chunk->rcvr->inqueue;
	struct sctp_ep_common *rcvr = NULL;
	int backloged = 0;

	rcvr = chunk->rcvr;

	 
	if (rcvr->dead) {
		sctp_chunk_free(chunk);
		goto done;
	}

	if (unlikely(rcvr->sk != sk)) {
		 

		sk = rcvr->sk;
		sctp_bh_lock_sock(sk);

		if (sock_owned_by_user(sk)) {
			sk_add_backlog(sk, skb);
			backloged = 1;
		} else
			sctp_inq_push(inqueue, chunk);

		sctp_bh_unlock_sock(sk);

		 
		if (backloged)
			return 0;
	} else {
		sctp_inq_push(inqueue, chunk);
	}

done:
	 
	if (SCTP_EP_TYPE_ASSOCIATION == rcvr->type)
		sctp_association_put(sctp_assoc(rcvr));
	else if (SCTP_EP_TYPE_SOCKET == rcvr->type)
		sctp_endpoint_put(sctp_ep(rcvr));
	else
		BUG();

	return 0;
}
