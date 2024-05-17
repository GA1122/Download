void process_packet_tail(struct msg_digest **mdp)
{
	struct msg_digest *md = *mdp;
	struct state *st = md->st;
	enum state_kind from_state = md->from_state;
	const struct state_microcode *smc = md->smc;
	bool new_iv_set = md->new_iv_set;
	bool self_delete = FALSE;

	if (md->hdr.isa_flags & ISAKMP_FLAGS_v1_ENCRYPTION) {
		DBG(DBG_CRYPT, {
			ipstr_buf b;
			DBG_log("received encrypted packet from %s:%u",
				ipstr(&md->sender, &b),
				(unsigned)md->sender_port);
		});

		if (st == NULL) {
			libreswan_log(
				"discarding encrypted message for an unknown ISAKMP SA");
			SEND_NOTIFICATION(PAYLOAD_MALFORMED  );
			return;
		}
		if (st->st_skey_ei_nss == NULL) {
			loglog(RC_LOG_SERIOUS,
				"discarding encrypted message because we haven't yet negotiated keying material");
			SEND_NOTIFICATION(INVALID_FLAGS);
			return;
		}

		 
		md->encrypted = TRUE;

		DBG(DBG_CRYPT,
		    DBG_log("decrypting %u bytes using algorithm %s",
			    (unsigned) pbs_left(&md->message_pbs),
			    enum_show(&oakley_enc_names,
				      st->st_oakley.encrypt)));

		 
		{
			const struct encrypt_desc *e = st->st_oakley.encrypter;

			if (pbs_left(&md->message_pbs) % e->enc_blocksize != 0)
			{
				loglog(RC_LOG_SERIOUS,
				       "malformed message: not a multiple of encryption blocksize");
				SEND_NOTIFICATION(PAYLOAD_MALFORMED);
				return;
			}

			 

			 
			clonetochunk(md->raw_packet, md->packet_pbs.start,
				     pbs_room(&md->packet_pbs), "raw packet");

			 
			if (!new_iv_set) {
				if (st->st_iv_len == 0) {
					init_phase2_iv(st, &md->hdr.isa_msgid);
				} else {
					 
					restore_new_iv(st, st->st_iv, st->st_iv_len);
				}
			}

			crypto_cbc_encrypt(e, FALSE, md->message_pbs.cur,
					   pbs_left(&md->message_pbs), st);

		}

		DBG_cond_dump(DBG_CRYPT, "decrypted:\n", md->message_pbs.cur,
			      md->message_pbs.roof - md->message_pbs.cur);

		DBG_cond_dump(DBG_CRYPT, "next IV:",
			      st->st_new_iv, st->st_new_iv_len);
	} else {
		 

		if (smc->flags & SMF_INPUT_ENCRYPTED) {
			loglog(RC_LOG_SERIOUS,
			       "packet rejected: should have been encrypted");
			SEND_NOTIFICATION(INVALID_FLAGS);
			return;
		}
	}

	 
	{
		struct payload_digest *pd = md->digest;
		enum next_payload_types_ikev1 np = md->hdr.isa_np;
		lset_t needed = smc->req_payloads;
		const char *excuse =
			LIN(SMF_PSK_AUTH | SMF_FIRST_ENCRYPTED_INPUT,
			    smc->flags) ?
			"probable authentication failure (mismatch of preshared secrets?): "
			:
			"";

		while (np != ISAKMP_NEXT_NONE) {
			struct_desc *sd = v1_payload_desc(np);

			if (pd == &md->digest[PAYLIMIT]) {
				loglog(RC_LOG_SERIOUS,
				       "more than %d payloads in message; ignored",
				       PAYLIMIT);
				SEND_NOTIFICATION(PAYLOAD_MALFORMED);
				return;
			}

			 
			if (st != NULL && st->st_connection != NULL &&
			    (st->st_connection->policy & POLICY_AGGRESSIVE) == LEMPTY)
			{
				switch (np) {
				case ISAKMP_NEXT_NATD_RFC:
				case ISAKMP_NEXT_NATOA_RFC:
					if ((st->hidden_variables.st_nat_traversal & NAT_T_WITH_RFC_VALUES) == LEMPTY) {
						 
						DBG(DBG_NATT,
						    DBG_log("st_nat_traversal was: %s",
							    bitnamesof(natt_bit_names,
								       st->hidden_variables.st_nat_traversal)));
						sd = NULL;
					}
					break;
				default:
					break;
				}
			}

			if (sd == NULL) {
				 
				switch (np) {
				case ISAKMP_NEXT_ID:
					 
					sd = (IS_PHASE1(from_state) ||
					      IS_PHASE15(from_state)) ?
						&isakmp_identification_desc :
						&isakmp_ipsec_identification_desc;
					break;

				case ISAKMP_NEXT_NATD_DRAFTS:
					 
					np = ISAKMP_NEXT_NATD_RFC;
					sd = v1_payload_desc(np);
					break;

				case ISAKMP_NEXT_NATOA_DRAFTS:
					 
					np = ISAKMP_NEXT_NATOA_RFC;
					sd = v1_payload_desc(np);
					break;

				case ISAKMP_NEXT_SAK:  
					 
					loglog(RC_LOG_SERIOUS,
						"%smessage with unsupported payload ISAKMP_NEXT_SAK (or ISAKMP_NEXT_NATD_BADDRAFTS) ignored",
						excuse);
					 
					if (!in_struct(&pd->payload, &isakmp_ignore_desc, &md->message_pbs,
						       &pd->pbs)) {
						loglog(RC_LOG_SERIOUS,
						       "%smalformed payload in packet",
						       excuse);
						SEND_NOTIFICATION(PAYLOAD_MALFORMED);
						return;
					}
					np = pd->payload.generic.isag_np;
					 
					continue;   

				default:
					loglog(RC_LOG_SERIOUS,
						"%smessage ignored because it contains an unknown or unexpected payload type (%s) at the outermost level",
					       excuse,
					       enum_show(&ikev1_payload_names, np));
					SEND_NOTIFICATION(INVALID_PAYLOAD_TYPE);
					return;
				}
				passert(sd != NULL);
			}

			passert(np < LELEM_ROOF);

			{
				lset_t s = LELEM(np);

				if (LDISJOINT(s,
					      needed | smc->opt_payloads |
					      LELEM(ISAKMP_NEXT_VID) |
					      LELEM(ISAKMP_NEXT_N) |
					      LELEM(ISAKMP_NEXT_D) |
					      LELEM(ISAKMP_NEXT_CR) |
					      LELEM(ISAKMP_NEXT_CERT))) {
					loglog(RC_LOG_SERIOUS, "%smessage ignored because it "
					       "contains an unexpected payload type (%s)",
					       excuse,
					       enum_show(&ikev1_payload_names, np));
					SEND_NOTIFICATION(INVALID_PAYLOAD_TYPE);
					return;
				}

				DBG(DBG_PARSING,
				    DBG_log("got payload 0x%" PRIxLSET"  (%s) needed: 0x%" PRIxLSET "opt: 0x%" PRIxLSET,
					    s, enum_show(&ikev1_payload_names, np),
					    needed, smc->opt_payloads));
				needed &= ~s;
			}

			if (!in_struct(&pd->payload, sd, &md->message_pbs,
				       &pd->pbs)) {
				loglog(RC_LOG_SERIOUS,
				       "%smalformed payload in packet",
				       excuse);
				SEND_NOTIFICATION(PAYLOAD_MALFORMED);
				return;
			}

			 
			switch (np) {
			case ISAKMP_NEXT_ID:
			case ISAKMP_NEXT_NATOA_RFC:
				 
				DBG(DBG_PARSING,
				    DBG_dump("     obj: ", pd->pbs.cur,
					     pbs_left(&pd->pbs)));
				break;
			default:
				break;
			}

			 
			{
				struct payload_digest **p;

				for (p = &md->chain[np]; *p != NULL;
				     p = &(*p)->next)
					;
				*p = pd;
				pd->next = NULL;
			}

			np = pd->payload.generic.isag_np;
			pd++;

			 
			excuse = "";
		}

		md->digest_roof = pd;

		DBG(DBG_PARSING, {
			    if (pbs_left(&md->message_pbs) != 0)
				    DBG_log("removing %d bytes of padding",
					    (int) pbs_left(&md->message_pbs));
		    });

		md->message_pbs.roof = md->message_pbs.cur;

		 

		if (needed != 0) {
			loglog(RC_LOG_SERIOUS,
			       "message for %s is missing payloads %s",
			       enum_show(&state_names, from_state),
			       bitnamesof(payload_name_ikev1, needed));
			SEND_NOTIFICATION(PAYLOAD_MALFORMED);
			return;
		}
	}

	 

	if (IS_PHASE1(from_state) || IS_PHASE15(from_state)) {
		 
		if (md->chain[ISAKMP_NEXT_SA] != NULL &&
		    md->hdr.isa_np != ISAKMP_NEXT_SA) {
			loglog(RC_LOG_SERIOUS,
			       "malformed Phase 1 message: does not start with an SA payload");
			SEND_NOTIFICATION(PAYLOAD_MALFORMED);
			return;
		}
	} else if (IS_QUICK(from_state)) {
		 

		if (md->hdr.isa_np != ISAKMP_NEXT_HASH) {
			loglog(RC_LOG_SERIOUS,
			       "malformed Quick Mode message: does not start with a HASH payload");
			SEND_NOTIFICATION(PAYLOAD_MALFORMED);
			return;
		}

		{
			struct payload_digest *p;
			int i;

			p = md->chain[ISAKMP_NEXT_SA];
			i = 1;
			while (p != NULL) {
				if (p != &md->digest[i]) {
					loglog(RC_LOG_SERIOUS,
					       "malformed Quick Mode message: SA payload is in wrong position");
					SEND_NOTIFICATION(PAYLOAD_MALFORMED);
					return;
				}
				p = p->next;
				i++;
			}
		}

		 
		{
			struct payload_digest *id = md->chain[ISAKMP_NEXT_ID];

			if (id != NULL) {
				if (id->next == NULL ||
				    id->next->next != NULL) {
					loglog(RC_LOG_SERIOUS, "malformed Quick Mode message:"
					       " if any ID payload is present,"
					       " there must be exactly two");
					SEND_NOTIFICATION(PAYLOAD_MALFORMED);
					return;
				}
				if (id + 1 != id->next) {
					loglog(RC_LOG_SERIOUS, "malformed Quick Mode message:"
					       " the ID payloads are not adjacent");
					SEND_NOTIFICATION(PAYLOAD_MALFORMED);
					return;
				}
			}
		}
	}

	 
	 
	{
		struct payload_digest *p = md->chain[ISAKMP_NEXT_N];

		while (p != NULL) {
			switch (p->payload.notification.isan_type) {

			case R_U_THERE:
			case R_U_THERE_ACK:
			case ISAKMP_N_CISCO_LOAD_BALANCE:
			case PAYLOAD_MALFORMED:
			case INVALID_MESSAGE_ID:
			case IPSEC_RESPONDER_LIFETIME:
				if (md->hdr.isa_xchg == ISAKMP_XCHG_INFO) {
					 
					break;
				}
				 
			default:
				if (st == NULL || (st != NULL &&
						   (st->st_connection->policy & POLICY_OPPORTUNISTIC))) {
					DBG(DBG_CONTROL, DBG_log(
					       "ignoring informational payload %s, no corresponding state",
					       enum_show(& ikev1_notify_names,
							 p->payload.
							 notification.isan_type)));
				} else {
					loglog(RC_LOG_SERIOUS,
					       "ignoring informational payload %s, msgid=%08" PRIx32 ", length=%d",
					       enum_show(&ikev1_notify_names,
							 p->payload.
							 notification.isan_type),
					       st->st_msgid,
					       p->payload.notification.isan_length);
					DBG_dump_pbs(&p->pbs);
				}
				if (st != NULL &&
				    st->st_connection->extra_debugging &
				    IMPAIR_DIE_ONINFO) {
					loglog(RC_LOG_SERIOUS,
					       "received and failed on unknown informational message");
					complete_v1_state_transition(mdp,
								     STF_FATAL);
					 
					return;
				}
			}
			DBG_cond_dump(DBG_PARSING, "info:", p->pbs.cur, pbs_left(
					      &p->pbs));

			p = p->next;
		}

		p = md->chain[ISAKMP_NEXT_D];
		while (p != NULL) {
			self_delete |= accept_delete(md, p);
			DBG_cond_dump(DBG_PARSING, "del:", p->pbs.cur, pbs_left(
					      &p->pbs));
			p = p->next;
		}

		p = md->chain[ISAKMP_NEXT_VID];
		while (p != NULL) {
			handle_vendorid(md, (char *)p->pbs.cur,
					pbs_left(&p->pbs), FALSE);
			p = p->next;
		}
	}

	if (self_delete) {
		accept_self_delete(md);
		st = md->st;	 
		 
	}

#if 0
	 

	 
	if ((md->chain[ISAKMP_NEXT_NATD_RFC] != NULL ||
	     md->chain[ISAKMP_NEXT_NATOA_RFC] != NULL) &&
	    (st->hidden_variables.st_nat_traversal & NAT_T_WITH_RFC_VALUES) == LEMPTY) {
		 
		loglog(RC_LOG_SERIOUS,
		       "message ignored because it contains a NAT payload, when we did not receive the appropriate VendorID");
		return;
	}
#endif

	 
	if (smc->first_out_payload != ISAKMP_NEXT_NONE)
		ikev1_echo_hdr(md, (smc->flags & SMF_OUTPUT_ENCRYPTED) != 0,
			 smc->first_out_payload);

	complete_v1_state_transition(mdp, smc->processor(md));
	 
}
