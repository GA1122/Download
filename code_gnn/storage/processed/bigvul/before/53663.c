int ip6_datagram_send_ctl(struct net *net, struct sock *sk,
			  struct msghdr *msg, struct flowi6 *fl6,
			  struct ipv6_txoptions *opt,
			  int *hlimit, int *tclass, int *dontfrag)
{
	struct in6_pktinfo *src_info;
	struct cmsghdr *cmsg;
	struct ipv6_rt_hdr *rthdr;
	struct ipv6_opt_hdr *hdr;
	int len;
	int err = 0;

	for_each_cmsghdr(cmsg, msg) {
		int addr_type;

		if (!CMSG_OK(msg, cmsg)) {
			err = -EINVAL;
			goto exit_f;
		}

		if (cmsg->cmsg_level != SOL_IPV6)
			continue;

		switch (cmsg->cmsg_type) {
		case IPV6_PKTINFO:
		case IPV6_2292PKTINFO:
		    {
			struct net_device *dev = NULL;

			if (cmsg->cmsg_len < CMSG_LEN(sizeof(struct in6_pktinfo))) {
				err = -EINVAL;
				goto exit_f;
			}

			src_info = (struct in6_pktinfo *)CMSG_DATA(cmsg);

			if (src_info->ipi6_ifindex) {
				if (fl6->flowi6_oif &&
				    src_info->ipi6_ifindex != fl6->flowi6_oif)
					return -EINVAL;
				fl6->flowi6_oif = src_info->ipi6_ifindex;
			}

			addr_type = __ipv6_addr_type(&src_info->ipi6_addr);

			rcu_read_lock();
			if (fl6->flowi6_oif) {
				dev = dev_get_by_index_rcu(net, fl6->flowi6_oif);
				if (!dev) {
					rcu_read_unlock();
					return -ENODEV;
				}
			} else if (addr_type & IPV6_ADDR_LINKLOCAL) {
				rcu_read_unlock();
				return -EINVAL;
			}

			if (addr_type != IPV6_ADDR_ANY) {
				int strict = __ipv6_addr_src_scope(addr_type) <= IPV6_ADDR_SCOPE_LINKLOCAL;
				if (!(inet_sk(sk)->freebind || inet_sk(sk)->transparent) &&
				    !ipv6_chk_addr(net, &src_info->ipi6_addr,
						   strict ? dev : NULL, 0) &&
				    !ipv6_chk_acast_addr_src(net, dev,
							     &src_info->ipi6_addr))
					err = -EINVAL;
				else
					fl6->saddr = src_info->ipi6_addr;
			}

			rcu_read_unlock();

			if (err)
				goto exit_f;

			break;
		    }

		case IPV6_FLOWINFO:
			if (cmsg->cmsg_len < CMSG_LEN(4)) {
				err = -EINVAL;
				goto exit_f;
			}

			if (fl6->flowlabel&IPV6_FLOWINFO_MASK) {
				if ((fl6->flowlabel^*(__be32 *)CMSG_DATA(cmsg))&~IPV6_FLOWINFO_MASK) {
					err = -EINVAL;
					goto exit_f;
				}
			}
			fl6->flowlabel = IPV6_FLOWINFO_MASK & *(__be32 *)CMSG_DATA(cmsg);
			break;

		case IPV6_2292HOPOPTS:
		case IPV6_HOPOPTS:
			if (opt->hopopt || cmsg->cmsg_len < CMSG_LEN(sizeof(struct ipv6_opt_hdr))) {
				err = -EINVAL;
				goto exit_f;
			}

			hdr = (struct ipv6_opt_hdr *)CMSG_DATA(cmsg);
			len = ((hdr->hdrlen + 1) << 3);
			if (cmsg->cmsg_len < CMSG_LEN(len)) {
				err = -EINVAL;
				goto exit_f;
			}
			if (!ns_capable(net->user_ns, CAP_NET_RAW)) {
				err = -EPERM;
				goto exit_f;
			}
			opt->opt_nflen += len;
			opt->hopopt = hdr;
			break;

		case IPV6_2292DSTOPTS:
			if (cmsg->cmsg_len < CMSG_LEN(sizeof(struct ipv6_opt_hdr))) {
				err = -EINVAL;
				goto exit_f;
			}

			hdr = (struct ipv6_opt_hdr *)CMSG_DATA(cmsg);
			len = ((hdr->hdrlen + 1) << 3);
			if (cmsg->cmsg_len < CMSG_LEN(len)) {
				err = -EINVAL;
				goto exit_f;
			}
			if (!ns_capable(net->user_ns, CAP_NET_RAW)) {
				err = -EPERM;
				goto exit_f;
			}
			if (opt->dst1opt) {
				err = -EINVAL;
				goto exit_f;
			}
			opt->opt_flen += len;
			opt->dst1opt = hdr;
			break;

		case IPV6_DSTOPTS:
		case IPV6_RTHDRDSTOPTS:
			if (cmsg->cmsg_len < CMSG_LEN(sizeof(struct ipv6_opt_hdr))) {
				err = -EINVAL;
				goto exit_f;
			}

			hdr = (struct ipv6_opt_hdr *)CMSG_DATA(cmsg);
			len = ((hdr->hdrlen + 1) << 3);
			if (cmsg->cmsg_len < CMSG_LEN(len)) {
				err = -EINVAL;
				goto exit_f;
			}
			if (!ns_capable(net->user_ns, CAP_NET_RAW)) {
				err = -EPERM;
				goto exit_f;
			}
			if (cmsg->cmsg_type == IPV6_DSTOPTS) {
				opt->opt_flen += len;
				opt->dst1opt = hdr;
			} else {
				opt->opt_nflen += len;
				opt->dst0opt = hdr;
			}
			break;

		case IPV6_2292RTHDR:
		case IPV6_RTHDR:
			if (cmsg->cmsg_len < CMSG_LEN(sizeof(struct ipv6_rt_hdr))) {
				err = -EINVAL;
				goto exit_f;
			}

			rthdr = (struct ipv6_rt_hdr *)CMSG_DATA(cmsg);

			switch (rthdr->type) {
#if IS_ENABLED(CONFIG_IPV6_MIP6)
			case IPV6_SRCRT_TYPE_2:
				if (rthdr->hdrlen != 2 ||
				    rthdr->segments_left != 1) {
					err = -EINVAL;
					goto exit_f;
				}
				break;
#endif
			default:
				err = -EINVAL;
				goto exit_f;
			}

			len = ((rthdr->hdrlen + 1) << 3);

			if (cmsg->cmsg_len < CMSG_LEN(len)) {
				err = -EINVAL;
				goto exit_f;
			}

			 
			if ((rthdr->hdrlen >> 1) != rthdr->segments_left) {
				err = -EINVAL;
				goto exit_f;
			}

			opt->opt_nflen += len;
			opt->srcrt = rthdr;

			if (cmsg->cmsg_type == IPV6_2292RTHDR && opt->dst1opt) {
				int dsthdrlen = ((opt->dst1opt->hdrlen+1)<<3);

				opt->opt_nflen += dsthdrlen;
				opt->dst0opt = opt->dst1opt;
				opt->dst1opt = NULL;
				opt->opt_flen -= dsthdrlen;
			}

			break;

		case IPV6_2292HOPLIMIT:
		case IPV6_HOPLIMIT:
			if (cmsg->cmsg_len != CMSG_LEN(sizeof(int))) {
				err = -EINVAL;
				goto exit_f;
			}

			*hlimit = *(int *)CMSG_DATA(cmsg);
			if (*hlimit < -1 || *hlimit > 0xff) {
				err = -EINVAL;
				goto exit_f;
			}

			break;

		case IPV6_TCLASS:
		    {
			int tc;

			err = -EINVAL;
			if (cmsg->cmsg_len != CMSG_LEN(sizeof(int)))
				goto exit_f;

			tc = *(int *)CMSG_DATA(cmsg);
			if (tc < -1 || tc > 0xff)
				goto exit_f;

			err = 0;
			*tclass = tc;

			break;
		    }

		case IPV6_DONTFRAG:
		    {
			int df;

			err = -EINVAL;
			if (cmsg->cmsg_len != CMSG_LEN(sizeof(int)))
				goto exit_f;

			df = *(int *)CMSG_DATA(cmsg);
			if (df < 0 || df > 1)
				goto exit_f;

			err = 0;
			*dontfrag = df;

			break;
		    }
		default:
			net_dbg_ratelimited("invalid cmsg type: %d\n",
					    cmsg->cmsg_type);
			err = -EINVAL;
			goto exit_f;
		}
	}

exit_f:
	return err;
}