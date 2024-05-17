void nlmsg_set_src(struct nl_msg *msg, struct sockaddr_nl *addr)
{
	memcpy(&msg->nm_src, addr, sizeof(*addr));
}
