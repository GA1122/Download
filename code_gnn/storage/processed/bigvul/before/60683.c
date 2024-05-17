static int sctp_msghdr_parse(const struct msghdr *msg, struct sctp_cmsgs *cmsgs)
{
	struct msghdr *my_msg = (struct msghdr *)msg;
	struct cmsghdr *cmsg;

	for_each_cmsghdr(cmsg, my_msg) {
		if (!CMSG_OK(my_msg, cmsg))
			return -EINVAL;

		 
		if (cmsg->cmsg_level != IPPROTO_SCTP)
			continue;

		 
		switch (cmsg->cmsg_type) {
		case SCTP_INIT:
			 
			if (cmsg->cmsg_len != CMSG_LEN(sizeof(struct sctp_initmsg)))
				return -EINVAL;

			cmsgs->init = CMSG_DATA(cmsg);
			break;

		case SCTP_SNDRCV:
			 
			if (cmsg->cmsg_len != CMSG_LEN(sizeof(struct sctp_sndrcvinfo)))
				return -EINVAL;

			cmsgs->srinfo = CMSG_DATA(cmsg);

			if (cmsgs->srinfo->sinfo_flags &
			    ~(SCTP_UNORDERED | SCTP_ADDR_OVER |
			      SCTP_SACK_IMMEDIATELY | SCTP_PR_SCTP_MASK |
			      SCTP_ABORT | SCTP_EOF))
				return -EINVAL;
			break;

		case SCTP_SNDINFO:
			 
			if (cmsg->cmsg_len != CMSG_LEN(sizeof(struct sctp_sndinfo)))
				return -EINVAL;

			cmsgs->sinfo = CMSG_DATA(cmsg);

			if (cmsgs->sinfo->snd_flags &
			    ~(SCTP_UNORDERED | SCTP_ADDR_OVER |
			      SCTP_SACK_IMMEDIATELY | SCTP_PR_SCTP_MASK |
			      SCTP_ABORT | SCTP_EOF))
				return -EINVAL;
			break;
		default:
			return -EINVAL;
		}
	}

	return 0;
}
