static inline int compare_netns(struct rtable *rt1, struct rtable *rt2)
{
	return net_eq(dev_net(rt1->dst.dev), dev_net(rt2->dst.dev));
}
