static void nl_fib_input(struct sk_buff *skb)
{
	struct net *net;
	struct fib_result_nl *frn;
	struct nlmsghdr *nlh;
	u32 portid;

	net = sock_net(skb->sk);
	nlh = nlmsg_hdr(skb);
	if (skb->len < NLMSG_HDRLEN || skb->len < nlh->nlmsg_len ||
	    nlmsg_len(nlh) < sizeof(*frn))
		return;

	skb = netlink_skb_clone(skb, GFP_KERNEL);
	if (!skb)
		return;
	nlh = nlmsg_hdr(skb);

	frn = (struct fib_result_nl *) nlmsg_data(nlh);
	nl_fib_lookup(net, frn);

	portid = NETLINK_CB(skb).portid;       
	NETLINK_CB(skb).portid = 0;         
	NETLINK_CB(skb).dst_group = 0;   
	netlink_unicast(net->ipv4.fibnl, skb, portid, MSG_DONTWAIT);
}