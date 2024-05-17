void sctp_v6_pf_init(void)
{
	 
	sctp_register_pf(&sctp_pf_inet6, PF_INET6);

	 
	sctp_register_af(&sctp_af_inet6);
}
