static int sctp_v4_add_protocol(void)
{
	 
	register_inetaddr_notifier(&sctp_inetaddr_notifier);

	 
	if (inet_add_protocol(&sctp_protocol, IPPROTO_SCTP) < 0)
		return -EAGAIN;

	return 0;
}