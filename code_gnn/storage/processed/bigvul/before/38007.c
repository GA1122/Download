int netif_rx(struct sk_buff *skb)
{
	int ret;

	 
	if (netpoll_rx(skb))
		return NET_RX_DROP;

	if (netdev_tstamp_prequeue)
		net_timestamp_check(skb);

#ifdef CONFIG_RPS
	{
		struct rps_dev_flow voidflow, *rflow = &voidflow;
		int cpu;

		rcu_read_lock();

		cpu = get_rps_cpu(skb->dev, skb, &rflow);
		if (cpu < 0)
			cpu = smp_processor_id();

		ret = enqueue_to_backlog(skb, cpu, &rflow->last_qtail);

		rcu_read_unlock();
	}
#else
	{
		unsigned int qtail;
		ret = enqueue_to_backlog(skb, get_cpu(), &qtail);
		put_cpu();
	}
#endif
	return ret;
}