void nlmsg_set_creds(struct nl_msg *msg, struct ucred *creds)
{
	memcpy(&msg->nm_creds, creds, sizeof(*creds));
	msg->nm_flags |= NL_MSG_CRED_PRESENT;
}
