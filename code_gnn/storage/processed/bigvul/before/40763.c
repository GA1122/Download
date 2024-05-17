static void __x25_destroy_socket(struct sock *sk)
{
	struct sk_buff *skb;

	x25_stop_heartbeat(sk);
	x25_stop_timer(sk);

	x25_remove_socket(sk);
	x25_clear_queues(sk);		 

	while ((skb = skb_dequeue(&sk->sk_receive_queue)) != NULL) {
		if (skb->sk != sk) {		 
			 
			skb->sk->sk_state = TCP_LISTEN;
			sock_set_flag(skb->sk, SOCK_DEAD);
			x25_start_heartbeat(skb->sk);
			x25_sk(skb->sk)->state = X25_STATE_0;
		}

		kfree_skb(skb);
	}

	if (sk_has_allocations(sk)) {
		 
		sk->sk_timer.expires  = jiffies + 10 * HZ;
		sk->sk_timer.function = x25_destroy_timer;
		sk->sk_timer.data = (unsigned long)sk;
		add_timer(&sk->sk_timer);
	} else {
		 
		__sock_put(sk);
	}
}