static struct sctp_association *__sctp_rcv_walk_lookup(struct sk_buff *skb,
				      const union sctp_addr *laddr,
				      struct sctp_transport **transportp)
{
	struct sctp_association *asoc = NULL;
	sctp_chunkhdr_t *ch;
	int have_auth = 0;
	unsigned int chunk_num = 1;
	__u8 *ch_end;

	 
	ch = (sctp_chunkhdr_t *) skb->data;
	do {
		 
		if (ntohs(ch->length) < sizeof(sctp_chunkhdr_t))
			break;

		ch_end = ((__u8 *)ch) + WORD_ROUND(ntohs(ch->length));
		if (ch_end > skb_tail_pointer(skb))
			break;

		switch(ch->type) {
		    case SCTP_CID_AUTH:
			    have_auth = chunk_num;
			    break;

		    case SCTP_CID_COOKIE_ECHO:
			     
			    if (have_auth == 1 && chunk_num == 2)
				    return NULL;
			    break;

		    case SCTP_CID_ASCONF:
			    if (have_auth || sctp_addip_noauth)
				    asoc = __sctp_rcv_asconf_lookup(ch, laddr,
							sctp_hdr(skb)->source,
							transportp);
		    default:
			    break;
		}

		if (asoc)
			break;

		ch = (sctp_chunkhdr_t *) ch_end;
		chunk_num++;
	} while (ch_end < skb_tail_pointer(skb));

	return asoc;
}
