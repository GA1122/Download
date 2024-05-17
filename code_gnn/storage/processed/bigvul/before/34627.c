void sctp_icmp_frag_needed(struct sock *sk, struct sctp_association *asoc,
			   struct sctp_transport *t, __u32 pmtu)
{
	if (!t || (t->pathmtu <= pmtu))
		return;

	if (sock_owned_by_user(sk)) {
		asoc->pmtu_pending = 1;
		t->pmtu_pending = 1;
		return;
	}

	if (t->param_flags & SPP_PMTUD_ENABLE) {
		 
		sctp_transport_update_pmtu(t, pmtu);

		 
		sctp_assoc_sync_pmtu(asoc);
	}

	 
	sctp_retransmit(&asoc->outqueue, t, SCTP_RTXR_PMTUD);
}
