static void sctp_cmd_delete_tcb(sctp_cmd_seq_t *cmds,
				struct sctp_association *asoc)
{
	struct sock *sk = asoc->base.sk;

	 
	if (sctp_style(sk, TCP) && sctp_sstate(sk, LISTENING) &&
	    (!asoc->temp) && (sk->sk_shutdown != SHUTDOWN_MASK))
		return;

	sctp_unhash_established(asoc);
	sctp_association_free(asoc);
}
