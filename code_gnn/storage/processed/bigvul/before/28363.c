void ping_err(struct sk_buff *skb, int offset, u32 info)
{
	int family;
	struct icmphdr *icmph;
	struct inet_sock *inet_sock;
	int type;
	int code;
	struct net *net = dev_net(skb->dev);
	struct sock *sk;
	int harderr;
	int err;

	if (skb->protocol == htons(ETH_P_IP)) {
		family = AF_INET;
		type = icmp_hdr(skb)->type;
		code = icmp_hdr(skb)->code;
		icmph = (struct icmphdr *)(skb->data + offset);
	} else if (skb->protocol == htons(ETH_P_IPV6)) {
		family = AF_INET6;
		type = icmp6_hdr(skb)->icmp6_type;
		code = icmp6_hdr(skb)->icmp6_code;
		icmph = (struct icmphdr *) (skb->data + offset);
	} else {
		BUG();
	}

	 

	if (!ping_supported(family, icmph->type, icmph->code))
		return;

	pr_debug("ping_err(proto=0x%x,type=%d,code=%d,id=%04x,seq=%04x)\n",
		 skb->protocol, type, code, ntohs(icmph->un.echo.id),
		 ntohs(icmph->un.echo.sequence));

	sk = ping_lookup(net, skb, ntohs(icmph->un.echo.id));
	if (sk == NULL) {
		pr_debug("no socket, dropping\n");
		return;	 
	}
	pr_debug("err on socket %p\n", sk);

	err = 0;
	harderr = 0;
	inet_sock = inet_sk(sk);

	if (skb->protocol == htons(ETH_P_IP)) {
		switch (type) {
		default:
		case ICMP_TIME_EXCEEDED:
			err = EHOSTUNREACH;
			break;
		case ICMP_SOURCE_QUENCH:
			 
			err = EREMOTEIO;
			break;
		case ICMP_PARAMETERPROB:
			err = EPROTO;
			harderr = 1;
			break;
		case ICMP_DEST_UNREACH:
			if (code == ICMP_FRAG_NEEDED) {  
				ipv4_sk_update_pmtu(skb, sk, info);
				if (inet_sock->pmtudisc != IP_PMTUDISC_DONT) {
					err = EMSGSIZE;
					harderr = 1;
					break;
				}
				goto out;
			}
			err = EHOSTUNREACH;
			if (code <= NR_ICMP_UNREACH) {
				harderr = icmp_err_convert[code].fatal;
				err = icmp_err_convert[code].errno;
			}
			break;
		case ICMP_REDIRECT:
			 
			ipv4_sk_redirect(skb, sk);
			err = EREMOTEIO;
			break;
		}
#if IS_ENABLED(CONFIG_IPV6)
	} else if (skb->protocol == htons(ETH_P_IPV6)) {
		harderr = pingv6_ops.icmpv6_err_convert(type, code, &err);
#endif
	}

	 
	if ((family == AF_INET && !inet_sock->recverr) ||
	    (family == AF_INET6 && !inet6_sk(sk)->recverr)) {
		if (!harderr || sk->sk_state != TCP_ESTABLISHED)
			goto out;
	} else {
		if (family == AF_INET) {
			ip_icmp_error(sk, skb, err, 0  ,
				      info, (u8 *)icmph);
#if IS_ENABLED(CONFIG_IPV6)
		} else if (family == AF_INET6) {
			pingv6_ops.ipv6_icmp_error(sk, skb, err, 0,
						   info, (u8 *)icmph);
#endif
		}
	}
	sk->sk_err = err;
	sk->sk_error_report(sk);
out:
	sock_put(sk);
}
