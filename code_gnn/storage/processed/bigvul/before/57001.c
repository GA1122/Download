static void sctp_cmd_new_state(sctp_cmd_seq_t *cmds,
			       struct sctp_association *asoc,
			       sctp_state_t state)
{
	struct sock *sk = asoc->base.sk;

	asoc->state = state;

	pr_debug("%s: asoc:%p[%s]\n", __func__, asoc, sctp_state_tbl[state]);

	if (sctp_style(sk, TCP)) {
		 
		if (sctp_state(asoc, ESTABLISHED) && sctp_sstate(sk, CLOSED))
			sk->sk_state = SCTP_SS_ESTABLISHED;

		 
		if (sctp_state(asoc, SHUTDOWN_RECEIVED) &&
		    sctp_sstate(sk, ESTABLISHED))
			sk->sk_shutdown |= RCV_SHUTDOWN;
	}

	if (sctp_state(asoc, COOKIE_WAIT)) {
		 
		asoc->timeouts[SCTP_EVENT_TIMEOUT_T1_INIT] =
						asoc->rto_initial;
		asoc->timeouts[SCTP_EVENT_TIMEOUT_T1_COOKIE] =
						asoc->rto_initial;
	}

	if (sctp_state(asoc, ESTABLISHED) ||
	    sctp_state(asoc, CLOSED) ||
	    sctp_state(asoc, SHUTDOWN_RECEIVED)) {
		 
		if (waitqueue_active(&asoc->wait))
			wake_up_interruptible(&asoc->wait);

		 
		if (!sctp_style(sk, UDP))
			sk->sk_state_change(sk);
	}
}
