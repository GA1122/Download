static stf_status ikev2_parent_outI1_common(struct msg_digest *md,
					    struct state *st)
{
	struct connection *c = st->st_connection;
	int numvidtosend = 0;

	 
	init_pbs(&reply_stream, reply_buffer, sizeof(reply_buffer),
		 "reply packet");

	 
	{
		struct isakmp_hdr hdr;

		zero(&hdr);                              
		 
		hdr.isa_version = build_ike_version();
		if (st->st_dcookie.ptr)
			hdr.isa_np   = ISAKMP_NEXT_v2N;
		else
			hdr.isa_np   = ISAKMP_NEXT_v2SA;
		hdr.isa_xchg = ISAKMP_v2_SA_INIT;
		hdr.isa_flags = ISAKMP_FLAGS_I;
		memcpy(hdr.isa_icookie, st->st_icookie, COOKIE_SIZE);
		 

		if (!out_struct(&hdr, &isakmp_hdr_desc, &reply_stream,
				&md->rbody)) {
			reset_cur_state();
			return STF_INTERNAL_ERROR;
		}
	}
	 

	if (st->st_dcookie.ptr) {
		chunk_t child_spi;
		memset(&child_spi, 0, sizeof(child_spi));
		ship_v2N(ISAKMP_NEXT_v2SA, DBGP(
				 IMPAIR_SEND_BOGUS_ISAKMP_FLAG) ?
			 (ISAKMP_PAYLOAD_NONCRITICAL |
			  ISAKMP_PAYLOAD_LIBRESWAN_BOGUS) :
			 ISAKMP_PAYLOAD_NONCRITICAL, PROTO_ISAKMP,
			 &child_spi,
			 v2N_COOKIE, &st->st_dcookie, &md->rbody);
	}
	 
	{
		u_char *sa_start = md->rbody.cur;

		if (st->st_sadb->prop_disj_cnt == 0 || st->st_sadb->prop_disj)
			st->st_sadb = sa_v2_convert(st->st_sadb);

		if (!ikev2_out_sa(&md->rbody,
				  PROTO_ISAKMP,
				  st->st_sadb,
				  st, TRUE,  
				  ISAKMP_NEXT_v2KE)) {
			libreswan_log("outsa fail");
			reset_cur_state();
			return STF_INTERNAL_ERROR;
		}
		 
		if (st->st_p1isa.ptr == NULL) {  
			clonetochunk(st->st_p1isa, sa_start,
				     md->rbody.cur - sa_start,
				     "sa in main_outI1");
		}
	}

	 
	if (!justship_v2KE(st, &st->st_gi, st->st_oakley.groupnum,  &md->rbody,
			   ISAKMP_NEXT_v2Ni))
		return STF_INTERNAL_ERROR;

	 
	if (c->send_vendorid) {
		numvidtosend++;   
	}

	 
	{
		int np = numvidtosend > 0 ? ISAKMP_NEXT_v2V : ISAKMP_NEXT_v2NONE;
		struct ikev2_generic in;
		pb_stream pb;

		memset(&in, 0, sizeof(in));
		in.isag_np = np;
		in.isag_critical = ISAKMP_PAYLOAD_NONCRITICAL;
		if (DBGP(IMPAIR_SEND_BOGUS_ISAKMP_FLAG)) {
			libreswan_log(
				" setting bogus ISAKMP_PAYLOAD_LIBRESWAN_BOGUS flag in ISAKMP payload");
			in.isag_critical |= ISAKMP_PAYLOAD_LIBRESWAN_BOGUS;
		}

		if (!out_struct(&in, &ikev2_nonce_desc, &md->rbody, &pb) ||
		    !out_raw(st->st_ni.ptr, st->st_ni.len, &pb, "IKEv2 nonce"))
			return STF_INTERNAL_ERROR;

		close_output_pbs(&pb);
	}

	 
	if (c->send_vendorid) {
		const char *myvid = ipsec_version_vendorid();
		int np = --numvidtosend >
			 0 ? ISAKMP_NEXT_v2V : ISAKMP_NEXT_v2NONE;

		if (!out_generic_raw(np, &isakmp_vendor_id_desc, &md->rbody,
				     myvid, strlen(myvid),
				     "Vendor ID"))
			return STF_INTERNAL_ERROR;

		 
		passert(numvidtosend == 0);
	}

	close_message(&md->rbody, st);
	close_output_pbs(&reply_stream);

	freeanychunk(st->st_tpacket);
	clonetochunk(st->st_tpacket, reply_stream.start,
		     pbs_offset(&reply_stream),
		     "reply packet for ikev2_parent_outI1_tail");

	 
	freeanychunk(st->st_firstpacket_me);
	clonetochunk(st->st_firstpacket_me, reply_stream.start,
		     pbs_offset(&reply_stream), "saved first packet");

	 
	send_ike_msg(st, __FUNCTION__);

	delete_event(st);
	event_schedule(EVENT_v2_RETRANSMIT, EVENT_RETRANSMIT_DELAY_0, st);

	reset_cur_state();
	return STF_OK;
}