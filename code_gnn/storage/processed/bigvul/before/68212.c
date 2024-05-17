void llc_sk_free(struct sock *sk)
{
	struct llc_sock *llc = llc_sk(sk);

	llc->state = LLC_CONN_OUT_OF_SVC;
	 
	llc_conn_ac_stop_all_timers(sk, NULL);
#ifdef DEBUG_LLC_CONN_ALLOC
	printk(KERN_INFO "%s: unackq=%d, txq=%d\n", __func__,
		skb_queue_len(&llc->pdu_unack_q),
		skb_queue_len(&sk->sk_write_queue));
#endif
	skb_queue_purge(&sk->sk_receive_queue);
	skb_queue_purge(&sk->sk_write_queue);
	skb_queue_purge(&llc->pdu_unack_q);
#ifdef LLC_REFCNT_DEBUG
	if (atomic_read(&sk->sk_refcnt) != 1) {
		printk(KERN_DEBUG "Destruction of LLC sock %p delayed in %s, cnt=%d\n",
			sk, __func__, atomic_read(&sk->sk_refcnt));
		printk(KERN_DEBUG "%d LLC sockets are still alive\n",
			atomic_read(&llc_sock_nr));
	} else {
		atomic_dec(&llc_sock_nr);
		printk(KERN_DEBUG "LLC socket %p released in %s, %d are still alive\n", sk,
			__func__, atomic_read(&llc_sock_nr));
	}
#endif
	sock_put(sk);
}