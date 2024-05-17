static int sctp_cmd_process_sack(sctp_cmd_seq_t *cmds,
				 struct sctp_association *asoc,
				 struct sctp_chunk *chunk)
{
	int err = 0;

	if (sctp_outq_sack(&asoc->outqueue, chunk)) {
		struct net *net = sock_net(asoc->base.sk);

		 
		err = sctp_do_sm(net, SCTP_EVENT_T_OTHER,
				 SCTP_ST_OTHER(SCTP_EVENT_NO_PENDING_TSN),
				 asoc->state, asoc->ep, asoc, NULL,
				 GFP_ATOMIC);
	}

	return err;
}