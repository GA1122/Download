 static stf_status ikev2_parent_inI1outR1_tail(
	struct pluto_crypto_req_cont *pcrc,
	struct pluto_crypto_req *r)
{
	struct ke_continuation *ke = (struct ke_continuation *)pcrc;
	struct msg_digest *md = ke->md;
	struct payload_digest *const sa_pd = md->chain[ISAKMP_NEXT_v2SA];
	struct state *const st = md->st;
	struct connection *c = st->st_connection;
	pb_stream *keyex_pbs;
	int numvidtosend = 0;

	if (c->send_vendorid) {
		numvidtosend++;  
	}
	 

	 
	clonetochunk(st->st_firstpacket_him, md->message_pbs.start,
		     pbs_offset(
			     &md->message_pbs), "saved first received packet");

	 
	zero(reply_buffer);
	init_pbs(&reply_stream, reply_buffer, sizeof(reply_buffer),
		 "reply packet");

	 
	{
		struct isakmp_hdr r_hdr = md->hdr;

		memcpy(r_hdr.isa_rcookie, st->st_rcookie, COOKIE_SIZE);
		r_hdr.isa_np = ISAKMP_NEXT_v2SA;
		 
		r_hdr.isa_version = build_ike_version();
		r_hdr.isa_flags &= ~ISAKMP_FLAGS_I;
		r_hdr.isa_flags |=  ISAKMP_FLAGS_R;
		 
		if (!out_struct(&r_hdr, &isakmp_hdr_desc, &reply_stream,
				&md->rbody))
			return STF_INTERNAL_ERROR;
	}

	 
	{
		struct isakmp_sa r_sa = sa_pd->payload.sa;
		v2_notification_t rn;
		pb_stream r_sa_pbs;

		r_sa.isasa_np = ISAKMP_NEXT_v2KE;  
		if (!out_struct(&r_sa, &ikev2_sa_desc, &md->rbody, &r_sa_pbs))
			return STF_INTERNAL_ERROR;

		 
		rn = ikev2_parse_parent_sa_body(&sa_pd->pbs,
						&sa_pd->payload.v2sa,
						&r_sa_pbs, st, FALSE);

		if (rn != v2N_NOTHING_WRONG)
			return STF_FAIL + rn;
	}

	{
		v2_notification_t rn;
		chunk_t dc;
		keyex_pbs = &md->chain[ISAKMP_NEXT_v2KE]->pbs;
		 
		rn =
			accept_KE(&st->st_gi, "Gi", st->st_oakley.group,
				  keyex_pbs);
		if (rn != v2N_NOTHING_WRONG) {
			u_int16_t group_number = htons(
				st->st_oakley.group->group);
			dc.ptr = (unsigned char *)&group_number;
			dc.len = 2;
			SEND_NOTIFICATION_AA(v2N_INVALID_KE_PAYLOAD, &dc);
			delete_state(st);
			return STF_FAIL + rn;
		}
	}

	 
	RETURN_STF_FAILURE(accept_v2_nonce(md, &st->st_ni, "Ni"));

	 
	if (!ship_v2KE(st, r, &st->st_gr, &md->rbody, ISAKMP_NEXT_v2Nr))
		return STF_INTERNAL_ERROR;

	 
	unpack_nonce(&st->st_nr, r);
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
		    !out_raw(st->st_nr.ptr, st->st_nr.len, &pb, "IKEv2 nonce"))
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
	}

	close_message(&md->rbody, st);
	close_output_pbs(&reply_stream);

	 
	freeanychunk(st->st_tpacket);
	clonetochunk(st->st_tpacket, reply_stream.start,
		     pbs_offset(&reply_stream),
		     "reply packet for ikev2_parent_inI1outR1_tail");

	 
	freeanychunk(st->st_firstpacket_me);
	clonetochunk(st->st_firstpacket_me, reply_stream.start,
		     pbs_offset(&reply_stream), "saved first packet");

	 

	return STF_OK;
}
