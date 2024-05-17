int x25_rx_call_request(struct sk_buff *skb, struct x25_neigh *nb,
			unsigned int lci)
{
	struct sock *sk;
	struct sock *make;
	struct x25_sock *makex25;
	struct x25_address source_addr, dest_addr;
	struct x25_facilities facilities;
	struct x25_dte_facilities dte_facilities;
	int len, addr_len, rc;

	 
	skb_pull(skb, X25_STD_MIN_LEN);

	 
	addr_len = x25_parse_address_block(skb, &source_addr, &dest_addr);
	if (addr_len <= 0)
		goto out_clear_request;
	skb_pull(skb, addr_len);

	 
	if (!pskb_may_pull(skb, 1))
		goto out_clear_request;
	len = skb->data[0] + 1;
	if (!pskb_may_pull(skb, len))
		goto out_clear_request;
	skb_pull(skb,len);

	 
	if (skb->len > X25_MAX_CUD_LEN)
		goto out_clear_request;

	 
	if (!pskb_may_pull(skb, skb->len))
		goto out_clear_request;

	 
	sk = x25_find_listener(&source_addr,skb);
	skb_push(skb,len);

	if (sk != NULL && sk_acceptq_is_full(sk)) {
		goto out_sock_put;
	}

	 
	if (sk == NULL) {
		skb_push(skb, addr_len + X25_STD_MIN_LEN);
		if (sysctl_x25_forward &&
				x25_forward_call(&dest_addr, nb, skb, lci) > 0)
		{
			 
			kfree_skb(skb);
			rc = 1;
			goto out;
		} else {
			 
			goto out_clear_request;
		}
	}

	 
	len = x25_negotiate_facilities(skb, sk, &facilities, &dte_facilities);
	if (len == -1)
		goto out_sock_put;

	 

	x25_limit_facilities(&facilities, nb);

	 
	make = x25_make_new(sk);
	if (!make)
		goto out_sock_put;

	 
	skb_pull(skb, len);

	skb->sk     = make;
	make->sk_state = TCP_ESTABLISHED;

	makex25 = x25_sk(make);
	makex25->lci           = lci;
	makex25->dest_addr     = dest_addr;
	makex25->source_addr   = source_addr;
	makex25->neighbour     = nb;
	makex25->facilities    = facilities;
	makex25->dte_facilities= dte_facilities;
	makex25->vc_facil_mask = x25_sk(sk)->vc_facil_mask;
	 
	makex25->vc_facil_mask &= ~X25_MASK_REVERSE;
	 
	makex25->vc_facil_mask &= ~X25_MASK_CALLING_AE;
	makex25->cudmatchlength = x25_sk(sk)->cudmatchlength;

	 
	if (test_bit(X25_ACCPT_APPRV_FLAG, &makex25->flags)) {
		x25_write_internal(make, X25_CALL_ACCEPTED);
		makex25->state = X25_STATE_3;
	}

	 
	skb_copy_from_linear_data(skb, makex25->calluserdata.cuddata, skb->len);
	makex25->calluserdata.cudlength = skb->len;

	sk->sk_ack_backlog++;

	x25_insert_socket(make);

	skb_queue_head(&sk->sk_receive_queue, skb);

	x25_start_heartbeat(make);

	if (!sock_flag(sk, SOCK_DEAD))
		sk->sk_data_ready(sk, skb->len);
	rc = 1;
	sock_put(sk);
out:
	return rc;
out_sock_put:
	sock_put(sk);
out_clear_request:
	rc = 0;
	x25_transmit_clear_request(nb, lci, 0x01);
	goto out;
}