static int ip_rt_bug(struct sk_buff *skb)
{
	printk(KERN_DEBUG "ip_rt_bug: %pI4 -> %pI4, %s\n",
		&ip_hdr(skb)->saddr, &ip_hdr(skb)->daddr,
		skb->dev ? skb->dev->name : "?");
	kfree_skb(skb);
	WARN_ON(1);
	return 0;
}
