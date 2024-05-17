static inline int nf_ingress(struct sk_buff *skb, struct packet_type **pt_prev,
			     int *ret, struct net_device *orig_dev)
{
#ifdef CONFIG_NETFILTER_INGRESS
	if (nf_hook_ingress_active(skb)) {
		if (*pt_prev) {
			*ret = deliver_skb(skb, *pt_prev, orig_dev);
			*pt_prev = NULL;
		}

		return nf_hook_ingress(skb);
	}
#endif  
	return 0;
}