void sctp_assoc_migrate(struct sctp_association *assoc, struct sock *newsk)
{
	struct sctp_sock *newsp = sctp_sk(newsk);
	struct sock *oldsk = assoc->base.sk;

	 
	list_del_init(&assoc->asocs);

	 
	if (sctp_style(oldsk, TCP))
		oldsk->sk_ack_backlog--;

	 
	sctp_endpoint_put(assoc->ep);
	sock_put(assoc->base.sk);

	 
	assoc->ep = newsp->ep;
	sctp_endpoint_hold(assoc->ep);

	 
	assoc->base.sk = newsk;
	sock_hold(assoc->base.sk);

	 
	sctp_endpoint_add_asoc(newsp->ep, assoc);
}
