static int l2tp_ip6_backlog_recv(struct sock *sk, struct sk_buff *skb)
{
	int rc;

	 
	rc = sock_queue_rcv_skb(sk, skb);
	if (rc < 0)
		goto drop;

	return 0;

drop:
	IP_INC_STATS(&init_net, IPSTATS_MIB_INDISCARDS);
	kfree_skb(skb);
	return -1;
}
