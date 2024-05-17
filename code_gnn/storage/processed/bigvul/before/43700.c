static void ndisc_error_report(struct neighbour *neigh, struct sk_buff *skb)
{
	 
	dst_link_failure(skb);
	kfree_skb(skb);
}
