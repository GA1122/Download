bool sk_busy_loop(struct sock *sk, int nonblock)
{
	unsigned long end_time = !nonblock ? sk_busy_loop_end_time(sk) : 0;
	int (*busy_poll)(struct napi_struct *dev);
	struct napi_struct *napi;
	int rc = false;

	rcu_read_lock();

	napi = napi_by_id(sk->sk_napi_id);
	if (!napi)
		goto out;

	 
	busy_poll = napi->dev->netdev_ops->ndo_busy_poll;

	do {
		rc = 0;
		local_bh_disable();
		if (busy_poll) {
			rc = busy_poll(napi);
		} else if (napi_schedule_prep(napi)) {
			void *have = netpoll_poll_lock(napi);

			if (test_bit(NAPI_STATE_SCHED, &napi->state)) {
				rc = napi->poll(napi, BUSY_POLL_BUDGET);
				trace_napi_poll(napi);
				if (rc == BUSY_POLL_BUDGET) {
					napi_complete_done(napi, rc);
					napi_schedule(napi);
				}
			}
			netpoll_poll_unlock(have);
		}
		if (rc > 0)
			NET_ADD_STATS_BH(sock_net(sk),
					 LINUX_MIB_BUSYPOLLRXPACKETS, rc);
		local_bh_enable();

		if (rc == LL_FLUSH_FAILED)
			break;  

		cpu_relax();
	} while (!nonblock && skb_queue_empty(&sk->sk_receive_queue) &&
		 !need_resched() && !busy_loop_timeout(end_time));

	rc = !skb_queue_empty(&sk->sk_receive_queue);
out:
	rcu_read_unlock();
	return rc;
}