static void pppol2tp_recv(struct l2tp_session *session, struct sk_buff *skb, int data_len)
{
	struct pppol2tp_session *ps = l2tp_session_priv(session);
	struct sock *sk = NULL;

	 
	sk = ps->sock;
	if (sk == NULL)
		goto no_sock;

	if (sk->sk_state & PPPOX_BOUND) {
		struct pppox_sock *po;
		l2tp_dbg(session, PPPOL2TP_MSG_DATA,
			 "%s: recv %d byte data frame, passing to ppp\n",
			 session->name, data_len);

		 
		secpath_reset(skb);
		skb_dst_drop(skb);
		nf_reset(skb);

		po = pppox_sk(sk);
		ppp_input(&po->chan, skb);
	} else {
		l2tp_info(session, PPPOL2TP_MSG_DATA, "%s: socket not bound\n",
			  session->name);

		 
		atomic_long_inc(&session->stats.rx_errors);
		kfree_skb(skb);
	}

	return;

no_sock:
	l2tp_info(session, PPPOL2TP_MSG_DATA, "%s: no socket\n", session->name);
	kfree_skb(skb);
}
