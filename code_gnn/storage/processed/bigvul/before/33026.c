SCTP_STATIC int sctp_msghdr_parse(const struct msghdr *msg,
				  sctp_cmsgs_t *cmsgs)
{
	struct cmsghdr *cmsg;
	struct msghdr *my_msg = (struct msghdr *)msg;

	for (cmsg = CMSG_FIRSTHDR(msg);
	     cmsg != NULL;
	     cmsg = CMSG_NXTHDR(my_msg, cmsg)) {
		if (!CMSG_OK(my_msg, cmsg))
			return -EINVAL;

		 
		if (cmsg->cmsg_level != IPPROTO_SCTP)
			continue;

		 
		switch (cmsg->cmsg_type) {
		case SCTP_INIT:
			 
			if (cmsg->cmsg_len !=
			    CMSG_LEN(sizeof(struct sctp_initmsg)))
				return -EINVAL;
			cmsgs->init = (struct sctp_initmsg *)CMSG_DATA(cmsg);
			break;

		case SCTP_SNDRCV:
			 
			if (cmsg->cmsg_len !=
			    CMSG_LEN(sizeof(struct sctp_sndrcvinfo)))
				return -EINVAL;

			cmsgs->info =
				(struct sctp_sndrcvinfo *)CMSG_DATA(cmsg);

			 
			if (cmsgs->info->sinfo_flags &
			    ~(SCTP_UNORDERED | SCTP_ADDR_OVER |
			      SCTP_ABORT | SCTP_EOF))
				return -EINVAL;
			break;

		default:
			return -EINVAL;
		}
	}
	return 0;
}
