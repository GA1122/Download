static void reqsk_timer_handler(unsigned long data)
{
	struct request_sock *req = (struct request_sock *)data;
	struct sock *sk_listener = req->rsk_listener;
	struct net *net = sock_net(sk_listener);
	struct inet_connection_sock *icsk = inet_csk(sk_listener);
	struct request_sock_queue *queue = &icsk->icsk_accept_queue;
	int qlen, expire = 0, resend = 0;
	int max_retries, thresh;
	u8 defer_accept;

	if (sk_state_load(sk_listener) != TCP_LISTEN)
		goto drop;

	max_retries = icsk->icsk_syn_retries ? : net->ipv4.sysctl_tcp_synack_retries;
	thresh = max_retries;
	 
	qlen = reqsk_queue_len(queue);
	if ((qlen << 1) > max(8U, sk_listener->sk_max_ack_backlog)) {
		int young = reqsk_queue_len_young(queue) << 1;

		while (thresh > 2) {
			if (qlen < young)
				break;
			thresh--;
			young <<= 1;
		}
	}
	defer_accept = READ_ONCE(queue->rskq_defer_accept);
	if (defer_accept)
		max_retries = defer_accept;
	syn_ack_recalc(req, thresh, max_retries, defer_accept,
		       &expire, &resend);
	req->rsk_ops->syn_ack_timeout(req);
	if (!expire &&
	    (!resend ||
	     !inet_rtx_syn_ack(sk_listener, req) ||
	     inet_rsk(req)->acked)) {
		unsigned long timeo;

		if (req->num_timeout++ == 0)
			atomic_dec(&queue->young);
		timeo = min(TCP_TIMEOUT_INIT << req->num_timeout, TCP_RTO_MAX);
		mod_timer(&req->rsk_timer, jiffies + timeo);
		return;
	}
drop:
	inet_csk_reqsk_queue_drop_and_put(sk_listener, req);
}
