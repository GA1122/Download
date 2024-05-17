static int sctp_get_port(struct sock *sk, unsigned short snum)
{
	long ret;
	union sctp_addr addr;
	struct sctp_af *af = sctp_sk(sk)->pf->af;

	 
	af->from_sk(&addr, sk);
	addr.v4.sin_port = htons(snum);

	 
	ret = sctp_get_port_local(sk, &addr);

	return (ret ? 1 : 0);
}
