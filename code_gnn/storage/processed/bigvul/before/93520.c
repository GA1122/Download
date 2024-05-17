static void ip6mr_cache_resolve(struct net *net, struct mr6_table *mrt,
				struct mfc6_cache *uc, struct mfc6_cache *c)
{
	struct sk_buff *skb;

	 

	while ((skb = __skb_dequeue(&uc->mfc_un.unres.unresolved))) {
		if (ipv6_hdr(skb)->version == 0) {
			struct nlmsghdr *nlh = (struct nlmsghdr *)skb_pull(skb, sizeof(struct ipv6hdr));

			if (__ip6mr_fill_mroute(mrt, skb, c, nlmsg_data(nlh)) > 0) {
				nlh->nlmsg_len = skb_tail_pointer(skb) - (u8 *)nlh;
			} else {
				nlh->nlmsg_type = NLMSG_ERROR;
				nlh->nlmsg_len = nlmsg_msg_size(sizeof(struct nlmsgerr));
				skb_trim(skb, nlh->nlmsg_len);
				((struct nlmsgerr *)nlmsg_data(nlh))->error = -EMSGSIZE;
			}
			rtnl_unicast(skb, net, NETLINK_CB(skb).portid);
		} else
			ip6_mr_forward(net, mrt, skb, c);
	}
}