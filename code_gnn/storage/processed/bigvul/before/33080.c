void sctp_write_space(struct sock *sk)
{
	struct sctp_association *asoc;

	 
	list_for_each_entry(asoc, &((sctp_sk(sk))->ep->asocs), asocs) {
		__sctp_write_space(asoc);
	}
}
