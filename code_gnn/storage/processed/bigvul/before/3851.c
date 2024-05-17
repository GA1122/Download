svc_dg_valid_pktinfo(struct msghdr *msg)
{
	struct cmsghdr *cmsg;

	if (!msg->msg_name)
		return 0;

	if (msg->msg_flags & MSG_CTRUNC)
		return 0;

	cmsg = CMSG_FIRSTHDR(msg);
	if (cmsg == NULL || CMSG_NXTHDR(msg, cmsg) != NULL)
		return 0;

	switch (((struct sockaddr *) msg->msg_name)->sa_family) {
	case AF_INET:
		if (cmsg->cmsg_level != SOL_IP
		 || cmsg->cmsg_type != IP_PKTINFO
		 || cmsg->cmsg_len < CMSG_LEN(sizeof (struct in_pktinfo))) {
			return 0;
		} else {
			struct in_pktinfo *pkti;
			
			pkti = (struct in_pktinfo *) CMSG_DATA (cmsg);
			pkti->ipi_ifindex = 0;
		}
		break;

#ifdef INET6
	case AF_INET6:
		if (cmsg->cmsg_level != SOL_IPV6
		 || cmsg->cmsg_type != IPV6_PKTINFO
		 || cmsg->cmsg_len < CMSG_LEN(sizeof (struct in6_pktinfo))) {
			return 0;
		} else {
			struct in6_pktinfo *pkti;
			
			pkti = (struct in6_pktinfo *) CMSG_DATA (cmsg);
			pkti->ipi6_ifindex = 0;
		}
		break;
#endif

	default:
		return 0;
	}

	return 1;
}
