void ax25_destroy_socket(ax25_cb *ax25)
{
	struct sk_buff *skb;

	ax25_cb_del(ax25);

	ax25_stop_heartbeat(ax25);
	ax25_stop_t1timer(ax25);
	ax25_stop_t2timer(ax25);
	ax25_stop_t3timer(ax25);
	ax25_stop_idletimer(ax25);

	ax25_clear_queues(ax25);	 

	if (ax25->sk != NULL) {
		while ((skb = skb_dequeue(&ax25->sk->sk_receive_queue)) != NULL) {
			if (skb->sk != ax25->sk) {
				 
				ax25_cb *sax25 = ax25_sk(skb->sk);

				 
				sock_orphan(skb->sk);

				 
				skb->sk->sk_state = TCP_LISTEN;

				ax25_start_heartbeat(sax25);
				sax25->state = AX25_STATE_0;
			}

			kfree_skb(skb);
		}
		skb_queue_purge(&ax25->sk->sk_write_queue);
	}

	if (ax25->sk != NULL) {
		if (sk_has_allocations(ax25->sk)) {
			 
			setup_timer(&ax25->dtimer, ax25_destroy_timer,
					(unsigned long)ax25);
			ax25->dtimer.expires  = jiffies + 2 * HZ;
			add_timer(&ax25->dtimer);
		} else {
			struct sock *sk=ax25->sk;
			ax25->sk=NULL;
			sock_put(sk);
		}
	} else {
		ax25_cb_put(ax25);
	}
}
