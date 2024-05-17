int llc_conn_state_process(struct sock *sk, struct sk_buff *skb)
{
	int rc;
	struct llc_sock *llc = llc_sk(skb->sk);
	struct llc_conn_state_ev *ev = llc_conn_ev(skb);

	 
	skb_get(skb);
	ev->ind_prim = ev->cfm_prim = 0;
	 
	rc = llc_conn_service(skb->sk, skb);
	if (unlikely(rc != 0)) {
		printk(KERN_ERR "%s: llc_conn_service failed\n", __func__);
		goto out_kfree_skb;
	}

	if (unlikely(!ev->ind_prim && !ev->cfm_prim)) {
		 
		if (!skb->next)
			goto out_kfree_skb;
		goto out_skb_put;
	}

	if (unlikely(ev->ind_prim && ev->cfm_prim))  
		skb_get(skb);

	switch (ev->ind_prim) {
	case LLC_DATA_PRIM:
		llc_save_primitive(sk, skb, LLC_DATA_PRIM);
		if (unlikely(sock_queue_rcv_skb(sk, skb))) {
			 
			printk(KERN_ERR "%s: sock_queue_rcv_skb failed!\n",
			       __func__);
			kfree_skb(skb);
		}
		break;
	case LLC_CONN_PRIM:
		 
		skb_queue_tail(&sk->sk_receive_queue, skb);
		sk->sk_state_change(sk);
		break;
	case LLC_DISC_PRIM:
		sock_hold(sk);
		if (sk->sk_type == SOCK_STREAM &&
		    sk->sk_state == TCP_ESTABLISHED) {
			sk->sk_shutdown       = SHUTDOWN_MASK;
			sk->sk_socket->state  = SS_UNCONNECTED;
			sk->sk_state          = TCP_CLOSE;
			if (!sock_flag(sk, SOCK_DEAD)) {
				sock_set_flag(sk, SOCK_DEAD);
				sk->sk_state_change(sk);
			}
		}
		kfree_skb(skb);
		sock_put(sk);
		break;
	case LLC_RESET_PRIM:
		 
		printk(KERN_INFO "%s: received a reset ind!\n", __func__);
		kfree_skb(skb);
		break;
	default:
		if (ev->ind_prim) {
			printk(KERN_INFO "%s: received unknown %d prim!\n",
				__func__, ev->ind_prim);
			kfree_skb(skb);
		}
		 
		break;
	}

	switch (ev->cfm_prim) {
	case LLC_DATA_PRIM:
		if (!llc_data_accept_state(llc->state))
			sk->sk_write_space(sk);
		else
			rc = llc->failed_data_req = 1;
		break;
	case LLC_CONN_PRIM:
		if (sk->sk_type == SOCK_STREAM &&
		    sk->sk_state == TCP_SYN_SENT) {
			if (ev->status) {
				sk->sk_socket->state = SS_UNCONNECTED;
				sk->sk_state         = TCP_CLOSE;
			} else {
				sk->sk_socket->state = SS_CONNECTED;
				sk->sk_state         = TCP_ESTABLISHED;
			}
			sk->sk_state_change(sk);
		}
		break;
	case LLC_DISC_PRIM:
		sock_hold(sk);
		if (sk->sk_type == SOCK_STREAM && sk->sk_state == TCP_CLOSING) {
			sk->sk_socket->state = SS_UNCONNECTED;
			sk->sk_state         = TCP_CLOSE;
			sk->sk_state_change(sk);
		}
		sock_put(sk);
		break;
	case LLC_RESET_PRIM:
		 
		printk(KERN_INFO "%s: received a reset conf!\n", __func__);
		break;
	default:
		if (ev->cfm_prim) {
			printk(KERN_INFO "%s: received unknown %d prim!\n",
					__func__, ev->cfm_prim);
			break;
		}
		goto out_skb_put;  
	}
out_kfree_skb:
	kfree_skb(skb);
out_skb_put:
	kfree_skb(skb);
	return rc;
}