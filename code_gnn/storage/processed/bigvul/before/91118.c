static int inet_rtm_valid_getroute_req(struct sk_buff *skb,
				       const struct nlmsghdr *nlh,
				       struct nlattr **tb,
				       struct netlink_ext_ack *extack)
{
	struct rtmsg *rtm;
	int i, err;

	if (nlh->nlmsg_len < nlmsg_msg_size(sizeof(*rtm))) {
		NL_SET_ERR_MSG(extack,
			       "ipv4: Invalid header for route get request");
		return -EINVAL;
	}

	if (!netlink_strict_get_check(skb))
		return nlmsg_parse(nlh, sizeof(*rtm), tb, RTA_MAX,
				   rtm_ipv4_policy, extack);

	rtm = nlmsg_data(nlh);
	if ((rtm->rtm_src_len && rtm->rtm_src_len != 32) ||
	    (rtm->rtm_dst_len && rtm->rtm_dst_len != 32) ||
	    rtm->rtm_table || rtm->rtm_protocol ||
	    rtm->rtm_scope || rtm->rtm_type) {
		NL_SET_ERR_MSG(extack, "ipv4: Invalid values in header for route get request");
		return -EINVAL;
	}

	if (rtm->rtm_flags & ~(RTM_F_NOTIFY |
			       RTM_F_LOOKUP_TABLE |
			       RTM_F_FIB_MATCH)) {
		NL_SET_ERR_MSG(extack, "ipv4: Unsupported rtm_flags for route get request");
		return -EINVAL;
	}

	err = nlmsg_parse_strict(nlh, sizeof(*rtm), tb, RTA_MAX,
				 rtm_ipv4_policy, extack);
	if (err)
		return err;

	if ((tb[RTA_SRC] && !rtm->rtm_src_len) ||
	    (tb[RTA_DST] && !rtm->rtm_dst_len)) {
		NL_SET_ERR_MSG(extack, "ipv4: rtm_src_len and rtm_dst_len must be 32 for IPv4");
		return -EINVAL;
	}

	for (i = 0; i <= RTA_MAX; i++) {
		if (!tb[i])
			continue;

		switch (i) {
		case RTA_IIF:
		case RTA_OIF:
		case RTA_SRC:
		case RTA_DST:
		case RTA_IP_PROTO:
		case RTA_SPORT:
		case RTA_DPORT:
		case RTA_MARK:
		case RTA_UID:
			break;
		default:
			NL_SET_ERR_MSG(extack, "ipv4: Unsupported attribute in route get request");
			return -EINVAL;
		}
	}

	return 0;
}
