static int rawv6_sendmsg(struct kiocb *iocb, struct sock *sk,
		   struct msghdr *msg, size_t len)
{
	struct ipv6_txoptions opt_space;
	struct sockaddr_in6 * sin6 = (struct sockaddr_in6 *) msg->msg_name;
	struct in6_addr *daddr, *final_p, final;
	struct inet_sock *inet = inet_sk(sk);
	struct ipv6_pinfo *np = inet6_sk(sk);
	struct raw6_sock *rp = raw6_sk(sk);
	struct ipv6_txoptions *opt = NULL;
	struct ip6_flowlabel *flowlabel = NULL;
	struct dst_entry *dst = NULL;
	struct flowi6 fl6;
	int addr_len = msg->msg_namelen;
	int hlimit = -1;
	int tclass = -1;
	int dontfrag = -1;
	u16 proto;
	int err;

	 
	if (len > INT_MAX)
		return -EMSGSIZE;

	 
	if (msg->msg_flags & MSG_OOB)
		return -EOPNOTSUPP;

	 
	memset(&fl6, 0, sizeof(fl6));

	fl6.flowi6_mark = sk->sk_mark;

	if (sin6) {
		if (addr_len < SIN6_LEN_RFC2133)
			return -EINVAL;

		if (sin6->sin6_family && sin6->sin6_family != AF_INET6)
			return -EAFNOSUPPORT;

		 
		proto = ntohs(sin6->sin6_port);

		if (!proto)
			proto = inet->inet_num;
		else if (proto != inet->inet_num)
			return -EINVAL;

		if (proto > 255)
			return -EINVAL;

		daddr = &sin6->sin6_addr;
		if (np->sndflow) {
			fl6.flowlabel = sin6->sin6_flowinfo&IPV6_FLOWINFO_MASK;
			if (fl6.flowlabel&IPV6_FLOWLABEL_MASK) {
				flowlabel = fl6_sock_lookup(sk, fl6.flowlabel);
				if (flowlabel == NULL)
					return -EINVAL;
				daddr = &flowlabel->dst;
			}
		}

		 
		if (sk->sk_state == TCP_ESTABLISHED &&
		    ipv6_addr_equal(daddr, &sk->sk_v6_daddr))
			daddr = &sk->sk_v6_daddr;

		if (addr_len >= sizeof(struct sockaddr_in6) &&
		    sin6->sin6_scope_id &&
		    __ipv6_addr_needs_scope_id(__ipv6_addr_type(daddr)))
			fl6.flowi6_oif = sin6->sin6_scope_id;
	} else {
		if (sk->sk_state != TCP_ESTABLISHED)
			return -EDESTADDRREQ;

		proto = inet->inet_num;
		daddr = &sk->sk_v6_daddr;
		fl6.flowlabel = np->flow_label;
	}

	if (fl6.flowi6_oif == 0)
		fl6.flowi6_oif = sk->sk_bound_dev_if;

	if (msg->msg_controllen) {
		opt = &opt_space;
		memset(opt, 0, sizeof(struct ipv6_txoptions));
		opt->tot_len = sizeof(struct ipv6_txoptions);

		err = ip6_datagram_send_ctl(sock_net(sk), sk, msg, &fl6, opt,
					    &hlimit, &tclass, &dontfrag);
		if (err < 0) {
			fl6_sock_release(flowlabel);
			return err;
		}
		if ((fl6.flowlabel&IPV6_FLOWLABEL_MASK) && !flowlabel) {
			flowlabel = fl6_sock_lookup(sk, fl6.flowlabel);
			if (flowlabel == NULL)
				return -EINVAL;
		}
		if (!(opt->opt_nflen|opt->opt_flen))
			opt = NULL;
	}
	if (opt == NULL)
		opt = np->opt;
	if (flowlabel)
		opt = fl6_merge_options(&opt_space, flowlabel, opt);
	opt = ipv6_fixup_options(&opt_space, opt);

	fl6.flowi6_proto = proto;
	err = rawv6_probe_proto_opt(&fl6, msg);
	if (err)
		goto out;

	if (!ipv6_addr_any(daddr))
		fl6.daddr = *daddr;
	else
		fl6.daddr.s6_addr[15] = 0x1;  
	if (ipv6_addr_any(&fl6.saddr) && !ipv6_addr_any(&np->saddr))
		fl6.saddr = np->saddr;

	final_p = fl6_update_dst(&fl6, opt, &final);

	if (!fl6.flowi6_oif && ipv6_addr_is_multicast(&fl6.daddr))
		fl6.flowi6_oif = np->mcast_oif;
	else if (!fl6.flowi6_oif)
		fl6.flowi6_oif = np->ucast_oif;
	security_sk_classify_flow(sk, flowi6_to_flowi(&fl6));

	dst = ip6_dst_lookup_flow(sk, &fl6, final_p, true);
	if (IS_ERR(dst)) {
		err = PTR_ERR(dst);
		goto out;
	}
	if (hlimit < 0) {
		if (ipv6_addr_is_multicast(&fl6.daddr))
			hlimit = np->mcast_hops;
		else
			hlimit = np->hop_limit;
		if (hlimit < 0)
			hlimit = ip6_dst_hoplimit(dst);
	}

	if (tclass < 0)
		tclass = np->tclass;

	if (dontfrag < 0)
		dontfrag = np->dontfrag;

	if (msg->msg_flags&MSG_CONFIRM)
		goto do_confirm;

back_from_confirm:
	if (inet->hdrincl)
		err = rawv6_send_hdrinc(sk, msg->msg_iov, len, &fl6, &dst, msg->msg_flags);
	else {
		lock_sock(sk);
		err = ip6_append_data(sk, ip_generic_getfrag, msg->msg_iov,
			len, 0, hlimit, tclass, opt, &fl6, (struct rt6_info*)dst,
			msg->msg_flags, dontfrag);

		if (err)
			ip6_flush_pending_frames(sk);
		else if (!(msg->msg_flags & MSG_MORE))
			err = rawv6_push_pending_frames(sk, &fl6, rp);
		release_sock(sk);
	}
done:
	dst_release(dst);
out:
	fl6_sock_release(flowlabel);
	return err<0?err:len;
do_confirm:
	dst_confirm(dst);
	if (!(msg->msg_flags & MSG_PROBE) || len)
		goto back_from_confirm;
	err = 0;
	goto done;
}