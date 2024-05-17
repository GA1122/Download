static int sctp_inet_af_supported(sa_family_t family, struct sctp_sock *sp)
{
	 
	return AF_INET == family;
}
