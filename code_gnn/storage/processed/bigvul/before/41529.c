static int inet_gro_complete(struct sk_buff *skb, int nhoff)
{
	__be16 newlen = htons(skb->len - nhoff);
	struct iphdr *iph = (struct iphdr *)(skb->data + nhoff);
	const struct net_offload *ops;
	int proto = iph->protocol;
	int err = -ENOSYS;

	if (skb->encapsulation)
		skb_set_inner_network_header(skb, nhoff);

	csum_replace2(&iph->check, iph->tot_len, newlen);
	iph->tot_len = newlen;

	rcu_read_lock();
	ops = rcu_dereference(inet_offloads[proto]);
	if (WARN_ON(!ops || !ops->callbacks.gro_complete))
		goto out_unlock;

	 
	err = ops->callbacks.gro_complete(skb, nhoff + sizeof(*iph));

out_unlock:
	rcu_read_unlock();

	return err;
}