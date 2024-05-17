struct nl_msg *nlmsg_inherit(struct nlmsghdr *hdr)
{
	struct nl_msg *nm;

	nm = nlmsg_alloc();
	if (nm && hdr) {
		struct nlmsghdr *new = nm->nm_nlh;

		new->nlmsg_type = hdr->nlmsg_type;
		new->nlmsg_flags = hdr->nlmsg_flags;
		new->nlmsg_seq = hdr->nlmsg_seq;
		new->nlmsg_pid = hdr->nlmsg_pid;
	}

	return nm;
}
