void sctp_write_space(struct sock *sk)
{
	struct sctp_association *asoc;
	struct list_head *pos;

	 
	list_for_each(pos, &((sctp_sk(sk))->ep->asocs)) {
		asoc = list_entry(pos, struct sctp_association, asocs);
		__sctp_write_space(asoc);
	}
}
