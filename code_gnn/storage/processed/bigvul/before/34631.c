static int sctp_rcv_ootb(struct sk_buff *skb)
{
	sctp_chunkhdr_t *ch;
	__u8 *ch_end;
	sctp_errhdr_t *err;

	ch = (sctp_chunkhdr_t *) skb->data;

	 
	do {
		 
		if (ntohs(ch->length) < sizeof(sctp_chunkhdr_t))
			break;

		ch_end = ((__u8 *)ch) + WORD_ROUND(ntohs(ch->length));
		if (ch_end > skb_tail_pointer(skb))
			break;

		 
		if (SCTP_CID_ABORT == ch->type)
			goto discard;

		 
		if (SCTP_CID_SHUTDOWN_COMPLETE == ch->type)
			goto discard;

		 
		if (SCTP_CID_INIT == ch->type && (void *)ch != skb->data)
			goto discard;

		 
		if (SCTP_CID_COOKIE_ACK == ch->type)
			goto discard;

		if (SCTP_CID_ERROR == ch->type) {
			sctp_walk_errors(err, ch) {
				if (SCTP_ERROR_STALE_COOKIE == err->cause)
					goto discard;
			}
		}

		ch = (sctp_chunkhdr_t *) ch_end;
	} while (ch_end < skb_tail_pointer(skb));

	return 0;

discard:
	return 1;
}
