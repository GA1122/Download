static int ipv6_get_saddr_eval(struct net *net,
			       struct ipv6_saddr_score *score,
			       struct ipv6_saddr_dst *dst,
			       int i)
{
	int ret;

	if (i <= score->rule) {
		switch (i) {
		case IPV6_SADDR_RULE_SCOPE:
			ret = score->scopedist;
			break;
		case IPV6_SADDR_RULE_PREFIX:
			ret = score->matchlen;
			break;
		default:
			ret = !!test_bit(i, score->scorebits);
		}
		goto out;
	}

	switch (i) {
	case IPV6_SADDR_RULE_INIT:
		 
		ret = !!score->ifa;
		break;
	case IPV6_SADDR_RULE_LOCAL:
		 
		ret = ipv6_addr_equal(&score->ifa->addr, dst->addr);
		break;
	case IPV6_SADDR_RULE_SCOPE:
		 
		ret = __ipv6_addr_src_scope(score->addr_type);
		if (ret >= dst->scope)
			ret = -ret;
		else
			ret -= 128;	 
		score->scopedist = ret;
		break;
	case IPV6_SADDR_RULE_PREFERRED:
	    {
		 
		u8 avoid = IFA_F_DEPRECATED;

		if (!ipv6_use_optimistic_addr(score->ifa->idev))
			avoid |= IFA_F_OPTIMISTIC;
		ret = ipv6_saddr_preferred(score->addr_type) ||
		      !(score->ifa->flags & avoid);
		break;
	    }
#ifdef CONFIG_IPV6_MIP6
	case IPV6_SADDR_RULE_HOA:
	    {
		 
		int prefhome = !(dst->prefs & IPV6_PREFER_SRC_COA);
		ret = !(score->ifa->flags & IFA_F_HOMEADDRESS) ^ prefhome;
		break;
	    }
#endif
	case IPV6_SADDR_RULE_OIF:
		 
		ret = (!dst->ifindex ||
		       dst->ifindex == score->ifa->idev->dev->ifindex);
		break;
	case IPV6_SADDR_RULE_LABEL:
		 
		ret = ipv6_addr_label(net,
				      &score->ifa->addr, score->addr_type,
				      score->ifa->idev->dev->ifindex) == dst->label;
		break;
	case IPV6_SADDR_RULE_PRIVACY:
	    {
		 
		int preftmp = dst->prefs & (IPV6_PREFER_SRC_PUBLIC|IPV6_PREFER_SRC_TMP) ?
				!!(dst->prefs & IPV6_PREFER_SRC_TMP) :
				score->ifa->idev->cnf.use_tempaddr >= 2;
		ret = (!(score->ifa->flags & IFA_F_TEMPORARY)) ^ preftmp;
		break;
	    }
	case IPV6_SADDR_RULE_ORCHID:
		 
		ret = !(ipv6_addr_orchid(&score->ifa->addr) ^
			ipv6_addr_orchid(dst->addr));
		break;
	case IPV6_SADDR_RULE_PREFIX:
		 
		ret = ipv6_addr_diff(&score->ifa->addr, dst->addr);
		if (ret > score->ifa->prefix_len)
			ret = score->ifa->prefix_len;
		score->matchlen = ret;
		break;
#ifdef CONFIG_IPV6_OPTIMISTIC_DAD
	case IPV6_SADDR_RULE_NOT_OPTIMISTIC:
		 
		ret = !(score->ifa->flags & IFA_F_OPTIMISTIC);
		break;
#endif
	default:
		ret = 0;
	}

	if (ret)
		__set_bit(i, score->scorebits);
	score->rule = i;
out:
	return ret;
}
