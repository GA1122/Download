void process_v1_packet(struct msg_digest **mdp)
{
	struct msg_digest *md = *mdp;
	const struct state_microcode *smc;
	bool new_iv_set = FALSE;
	struct state *st = NULL;
	enum state_kind from_state = STATE_UNDEFINED;    

#define SEND_NOTIFICATION(t) { \
		if (st != NULL) \
			send_notification_from_state(st, from_state, t); \
		else \
			send_notification_from_md(md, t); }

	switch (md->hdr.isa_xchg) {
#ifdef NOTYET
	case ISAKMP_XCHG_NONE:
	case ISAKMP_XCHG_BASE:
	case ISAKMP_XCHG_AO:
#endif

	case ISAKMP_XCHG_AGGR:
	case ISAKMP_XCHG_IDPROT:  
		if (md->hdr.isa_msgid != v1_MAINMODE_MSGID) {
			libreswan_log(
				"Message ID was 0x%08lx but should be zero in phase 1",
				(unsigned long) md->hdr.isa_msgid);
			SEND_NOTIFICATION(INVALID_MESSAGE_ID);
			return;
		}

		if (is_zero_cookie(md->hdr.isa_icookie)) {
			libreswan_log(
				"Initiator Cookie must not be zero in phase 1 message");
			SEND_NOTIFICATION(INVALID_COOKIE);
			return;
		}

		if (is_zero_cookie(md->hdr.isa_rcookie)) {
			 
			if (md->hdr.isa_flags & ISAKMP_FLAGS_v1_ENCRYPTION) {
				libreswan_log("initial phase 1 message is invalid:"
					      " its Encrypted Flag is on");
				SEND_NOTIFICATION(INVALID_FLAGS);
				return;
			}

			 
			from_state = (md->hdr.isa_xchg == ISAKMP_XCHG_IDPROT ?
				      STATE_MAIN_R0 : STATE_AGGR_R0);
		} else {
			 

			st = find_state_ikev1(md->hdr.isa_icookie,
					      md->hdr.isa_rcookie,
					      md->hdr.isa_msgid);

			if (st == NULL) {
				 
				st = find_state_ikev1(md->hdr.isa_icookie,
						      zero_cookie,
						      md->hdr.isa_msgid);

				if (st == NULL) {
					libreswan_log(
						"phase 1 message is part of an unknown exchange");
					 
					return;
				}
			}
			set_cur_state(st);
			from_state = st->st_state;
		}
		break;

	case ISAKMP_XCHG_INFO:   
		st = ikev1_find_info_state(md->hdr.isa_icookie, md->hdr.isa_rcookie,
				     &md->sender, v1_MAINMODE_MSGID);

		if (st == NULL) {
			 
			st = find_state_ikev1(md->hdr.isa_icookie, zero_cookie,
					      v1_MAINMODE_MSGID);
		}

		if (st != NULL)
			set_cur_state(st);

		if (md->hdr.isa_flags & ISAKMP_FLAGS_v1_ENCRYPTION) {
			bool quiet = (st == NULL ||
				     (st->st_connection->policy & POLICY_OPPORTUNISTIC));

			if (st == NULL) {
				DBG(DBG_CONTROL, DBG_log(
						"Informational Exchange is for an unknown (expired?) SA with MSGID:0x%08lx",
							(unsigned long)md->hdr.isa_msgid));

				 
				DBG(DBG_CONTROL, {
					    DBG_dump("- unknown SA's md->hdr.isa_icookie:",
						    md->hdr.isa_icookie,
						    COOKIE_SIZE);
					    DBG_dump("- unknown SA's md->hdr.isa_rcookie:",
						    md->hdr.isa_rcookie,
						    COOKIE_SIZE);
				    });

				 
				return;
			}

			if (!IS_ISAKMP_ENCRYPTED(st->st_state)) {
				if (!quiet) {
					loglog(RC_LOG_SERIOUS, "encrypted Informational Exchange message is invalid because no key is known");
				}
				 
				return;
			}

			if (md->hdr.isa_msgid == v1_MAINMODE_MSGID) {
				if (!quiet) {
					loglog(RC_LOG_SERIOUS, "Informational Exchange message is invalid because it has a Message ID of 0");
				}
				 
				return;
			}

			if (!unique_msgid(st, md->hdr.isa_msgid)) {
				if (!quiet) {
					loglog(RC_LOG_SERIOUS, "Informational Exchange message is invalid because it has a previously used Message ID (0x%08lx)",
						(unsigned long)md->hdr.isa_msgid);
				}
				 
				return;
			}
			st->st_msgid_reserved = FALSE;

			init_phase2_iv(st, &md->hdr.isa_msgid);
			new_iv_set = TRUE;

			from_state = STATE_INFO_PROTECTED;
		} else {
			if (st != NULL &&
			    IS_ISAKMP_AUTHENTICATED(st->st_state)) {
				if ((st->st_connection->policy & POLICY_OPPORTUNISTIC) == LEMPTY) {
					loglog(RC_LOG_SERIOUS, "Informational Exchange message must be encrypted");
				}
				 
				return;
			}
			from_state = STATE_INFO;
		}
		break;

	case ISAKMP_XCHG_QUICK:  

		if (is_zero_cookie(md->hdr.isa_icookie)) {
			DBG(DBG_CONTROL, DBG_log(
				"Quick Mode message is invalid because it has an Initiator Cookie of 0"));
			SEND_NOTIFICATION(INVALID_COOKIE);
			return;
		}

		if (is_zero_cookie(md->hdr.isa_rcookie)) {
			DBG(DBG_CONTROL, DBG_log(
				"Quick Mode message is invalid because it has a Responder Cookie of 0"));
			SEND_NOTIFICATION(INVALID_COOKIE);
			return;
		}

		if (md->hdr.isa_msgid == v1_MAINMODE_MSGID) {
			DBG(DBG_CONTROL, DBG_log(
				"Quick Mode message is invalid because it has a Message ID of 0"));
			SEND_NOTIFICATION(INVALID_MESSAGE_ID);
			return;
		}

		st = find_state_ikev1(md->hdr.isa_icookie, md->hdr.isa_rcookie,
				      md->hdr.isa_msgid);

		if (st == NULL) {
			 
			st = find_state_ikev1(md->hdr.isa_icookie,
					      md->hdr.isa_rcookie,
					      v1_MAINMODE_MSGID);

			if (st == NULL) {
				DBG(DBG_CONTROL, DBG_log(
					"Quick Mode message is for a non-existent (expired?) ISAKMP SA"));
				 
				return;
			}

			if (st->st_oakley.doing_xauth) {
				DBG(DBG_CONTROL, DBG_log(
					"Cannot do Quick Mode until XAUTH done."));
				return;
			}

			 
			if (st->st_state == STATE_MODE_CFG_R2) {
				 
				change_state(st, STATE_MAIN_R3);
			}

#ifdef SOFTREMOTE_CLIENT_WORKAROUND
			 
			if (st->st_state == STATE_MODE_CFG_R1) {
				libreswan_log(
					"SoftRemote workaround: Cannot do Quick Mode until MODECFG done.");
				return;
			}
#endif

			set_cur_state(st);

			if (!IS_ISAKMP_SA_ESTABLISHED(st->st_state)) {
				if (DBGP(DBG_OPPO) || (st->st_connection->policy & POLICY_OPPORTUNISTIC) == LEMPTY) {
					loglog(RC_LOG_SERIOUS, "Quick Mode message is unacceptable because it is for an incomplete ISAKMP SA");
				}
				SEND_NOTIFICATION(PAYLOAD_MALFORMED  );
				return;
			}

			if (!unique_msgid(st, md->hdr.isa_msgid)) {
				if (DBGP(DBG_OPPO) || (st->st_connection->policy & POLICY_OPPORTUNISTIC) == LEMPTY) {
					loglog(RC_LOG_SERIOUS, "Quick Mode I1 message is unacceptable because it uses a previously used Message ID 0x%08lx (perhaps this is a duplicated packet)",
						(unsigned long) md->hdr.isa_msgid);
				}
				SEND_NOTIFICATION(INVALID_MESSAGE_ID);
				return;
			}
			st->st_msgid_reserved = FALSE;

			 
			init_phase2_iv(st, &md->hdr.isa_msgid);
			new_iv_set = TRUE;

			from_state = STATE_QUICK_R0;
		} else {
			if (st->st_oakley.doing_xauth) {
				if (DBGP(DBG_OPPO) ||
				    (st->st_connection->policy & POLICY_OPPORTUNISTIC) == LEMPTY) {
					libreswan_log("Cannot do Quick Mode until XAUTH done.");
				}
				return;
			}
			set_cur_state(st);
			from_state = st->st_state;
		}

		break;

	case ISAKMP_XCHG_MODE_CFG:
		if (is_zero_cookie(md->hdr.isa_icookie)) {
			DBG(DBG_CONTROL, DBG_log("Mode Config message is invalid because it has an Initiator Cookie of 0"));
			 
			return;
		}

		if (is_zero_cookie(md->hdr.isa_rcookie)) {
			DBG(DBG_CONTROL, DBG_log("Mode Config message is invalid because it has a Responder Cookie of 0"));
			 
			return;
		}

		if (md->hdr.isa_msgid == 0) {
			DBG(DBG_CONTROL, DBG_log("Mode Config message is invalid because it has a Message ID of 0"));
			 
			return;
		}

		st = ikev1_find_info_state(md->hdr.isa_icookie, md->hdr.isa_rcookie,
				     &md->sender, md->hdr.isa_msgid);

		if (st == NULL) {
			DBG(DBG_CONTROL, DBG_log(
				"No appropriate Mode Config state yet.See if we have a Main Mode state"));
			 
			st = ikev1_find_info_state(md->hdr.isa_icookie,
					     md->hdr.isa_rcookie,
					     &md->sender, 0);

			if (st == NULL) {
				DBG(DBG_CONTROL, DBG_log(
					"Mode Config message is for a non-existent (expired?) ISAKMP SA"));
				 
				return;
			}

			set_cur_state(st);

			DBG(DBG_CONTROLMORE, DBG_log(" processing received "
						     "isakmp_xchg_type %s.",
						     enum_show(&ikev1_exchange_names,
							       md->hdr.isa_xchg)));
			DBG(DBG_CONTROLMORE, DBG_log(" this is a%s%s%s%s",
						     st->st_connection->spd.
						     this.xauth_server ?
						     " xauthserver" : "",
						     st->st_connection->spd.
						     this.xauth_client ?
						     " xauthclient" : "",
						     st->st_connection->spd.
						     this.modecfg_server ?
						     " modecfgserver" : "",
						     st->st_connection->spd.
						     this.modecfg_client  ?
						     " modecfgclient" : ""
						     ));

			if (!IS_ISAKMP_SA_ESTABLISHED(st->st_state)) {
				DBG(DBG_CONTROLMORE, DBG_log(
					"Mode Config message is unacceptable because it is for an incomplete ISAKMP SA (state=%s)",
				       enum_name(&state_names, st->st_state)));
				 
				return;
			}
			DBG(DBG_CONTROLMORE, DBG_log(" call  init_phase2_iv"));
			init_phase2_iv(st, &md->hdr.isa_msgid);
			new_iv_set = TRUE;

			 

			if (st->st_connection->spd.this.xauth_server &&
			    st->st_state == STATE_XAUTH_R1 &&
			    st->quirks.xauth_ack_msgid) {
				from_state = STATE_XAUTH_R1;
				DBG(DBG_CONTROLMORE, DBG_log(
					" set from_state to %s state is STATE_XAUTH_R1 and quirks.xauth_ack_msgid is TRUE",
					    enum_name(&state_names,
						      st->st_state
						      )));
			} else if (st->st_connection->spd.this.xauth_client
				   &&
				   IS_PHASE1(st->st_state)) {
				from_state = STATE_XAUTH_I0;
				DBG(DBG_CONTROLMORE, DBG_log(
					" set from_state to %s this is xauthclient and IS_PHASE1() is TRUE",
					    enum_name(&state_names,
						      st->st_state
						      )));
			} else if (st->st_connection->spd.this.xauth_client
				   &&
				   st->st_state == STATE_XAUTH_I1) {
				 
				from_state = STATE_XAUTH_I0;
				DBG(DBG_CONTROLMORE, DBG_log(
					" set from_state to %s this is xauthclient and state == STATE_XAUTH_I1",
					    enum_name(&state_names,
						      st->st_state
						      )));
			} else if (st->st_connection->spd.this.modecfg_server
				   &&
				   IS_PHASE1(st->st_state)) {
				from_state = STATE_MODE_CFG_R0;
				DBG(DBG_CONTROLMORE, DBG_log(
					" set from_state to %s this is modecfgserver and IS_PHASE1() is TRUE",
					    enum_name(&state_names,
						      st->st_state
						      )));
			} else if (st->st_connection->spd.this.modecfg_client
				   &&
				   IS_PHASE1(st->st_state)) {
				from_state = STATE_MODE_CFG_R1;
				DBG(DBG_CONTROLMORE, DBG_log(
					" set from_state to %s this is modecfgclient and IS_PHASE1() is TRUE",
					    enum_name(&state_names,
						      st->st_state
						      )));
			} else {
				DBG(DBG_CONTROLMORE, DBG_log(
					"received isakmp_xchg_type %s",
					    enum_show(&ikev1_exchange_names,
						      md->hdr.isa_xchg)));
				DBG(DBG_CONTROLMORE, DBG_log(
					"this is a%s%s%s%s in state %s. Reply with UNSUPPORTED_EXCHANGE_TYPE",
					    st->st_connection
					    ->spd.this.xauth_server ?
					    " xauthserver" : "",
					    st->st_connection
					    ->spd.this.xauth_client ?
					    " xauthclient" : "",
					    st->st_connection
					    ->spd.this.modecfg_server ?
					    " modecfgserver" :
					    "",
					    st->st_connection
					    ->spd.this.modecfg_client  ?
					    " modecfgclient" :
					    "",
					    enum_name(&
						      state_names,
						      st->st_state)
					    ));
				return;
			}
		} else {
			if (st->st_connection->spd.this.xauth_server &&
			    IS_PHASE1(st->st_state)) {
				 
				DBG(DBG_CONTROL, DBG_log(
					"We were in phase 1, with no state, so we went to XAUTH_R0"));
				change_state(st, STATE_XAUTH_R0);
			}

			 
			set_cur_state(st);
			from_state = st->st_state;
		}

		break;

	case ISAKMP_XCHG_NGRP:
	default:
		DBG(DBG_CONTROL, DBG_log("unsupported exchange type %s in message",
			      enum_show(&ikev1_exchange_names, md->hdr.isa_xchg)));
		SEND_NOTIFICATION(UNSUPPORTED_EXCHANGE_TYPE);
		return;
	}

	 

	 
	if (md->hdr.isa_flags & ISAKMP_FLAGS_v1_COMMIT)
		DBG(DBG_CONTROL, DBG_log(
			"IKE message has the Commit Flag set but Pluto doesn't implement this feature due to security concerns; ignoring flag"));


	 
	if (md->hdr.isa_np == ISAKMP_NEXT_IKE_FRAGMENTATION) {
		struct isakmp_ikefrag fraghdr;
		struct ike_frag *ike_frag, **i;
		int last_frag_index = 0;   
		pb_stream frag_pbs;

		if (st == NULL) {
			DBG(DBG_CONTROL, DBG_log(
				"received IKE fragment, but have no state. Ignoring packet."));
			return;
		}

		if ((st->st_connection->policy & POLICY_IKE_FRAG_ALLOW) == 0) {
			DBG(DBG_CONTROL, DBG_log(
			       "discarding IKE fragment packet - fragmentation not allowed by local policy (ike_frag=no)"));
			return;
		}

		if (!in_struct(&fraghdr, &isakmp_ikefrag_desc,
			       &md->message_pbs, &frag_pbs) ||
		    pbs_room(&frag_pbs) != fraghdr.isafrag_length ||
		    fraghdr.isafrag_np != 0 ||
		    fraghdr.isafrag_number == 0 || fraghdr.isafrag_number >
		    16) {
			SEND_NOTIFICATION(PAYLOAD_MALFORMED);
			return;
		}

		DBG(DBG_CONTROL,
		    DBG_log("received IKE fragment id '%d', number '%u'%s",
			    fraghdr.isafrag_id,
			    fraghdr.isafrag_number,
			    (fraghdr.isafrag_flags == 1) ? "(last)" : ""));

		ike_frag = alloc_thing(struct ike_frag, "ike_frag");
		ike_frag->md = md;
		ike_frag->index = fraghdr.isafrag_number;
		ike_frag->last = (fraghdr.isafrag_flags & 1);
		ike_frag->size = pbs_left(&frag_pbs);
		ike_frag->data = frag_pbs.cur;

#if 0
 
		 
		if (md->iface->ike_float && ike_frag->index == 1 &&
		    (ike_frag->size >= NON_ESP_MARKER_SIZE &&
		     memeq(non_ESP_marker, ike_frag->data,
			    NON_ESP_MARKER_SIZE))) {
			ike_frag->data += NON_ESP_MARKER_SIZE;
			ike_frag->size -= NON_ESP_MARKER_SIZE;
		}
#endif

		 
		i = &st->ike_frags;
		for (;;) {
			if (ike_frag != NULL) {
				 
				if (*i == NULL ||
				    (*i)->index > ike_frag->index) {
					ike_frag->next = *i;
					*i = ike_frag;
					ike_frag = NULL;
				} else if ((*i)->index == ike_frag->index) {
					 
					struct ike_frag *old = *i;

					ike_frag->next = old->next;
					*i = ike_frag;
					release_md(old->md);
					pfree(old);
					ike_frag = NULL;
				}
			}

			if (*i == NULL)
				break;

			if ((*i)->last)
				last_frag_index = (*i)->index;

			i = &(*i)->next;
		}

		 
		if (last_frag_index != 0) {
			size_t size = 0;
			int prev_index = 0;
			struct ike_frag *frag;

			for (frag = st->ike_frags; frag; frag = frag->next) {
				size += frag->size;
				if (frag->index != ++prev_index) {
					break;  
				} else if (frag->index == last_frag_index) {
					struct msg_digest *whole_md = alloc_md();
					u_int8_t *buffer = alloc_bytes(size,
								       "IKE fragments buffer");
					size_t offset = 0;

					whole_md->iface = frag->md->iface;
					whole_md->sender = frag->md->sender;
					whole_md->sender_port =
						frag->md->sender_port;

					 
					frag = st->ike_frags;
					while (frag != NULL &&
					       frag->index <= last_frag_index)
					{
						passert(offset + frag->size <=
							size);
						memcpy(buffer + offset,
						       frag->data, frag->size);
						offset += frag->size;
						frag = frag->next;
					}

					init_pbs(&whole_md->packet_pbs, buffer, size,
						 "packet");

					process_packet(&whole_md);
					release_any_md(&whole_md);
					release_fragments(st);
					 
					st->st_seen_fragments = TRUE;
					DBG(DBG_CONTROL, DBG_log(
						" updated IKE fragment state to respond using fragments without waiting for re-transmits"));
					break;
				}
			}
		}

		 
		 
		*mdp = NULL;
		return;
	}

	 
	passert(STATE_IKE_FLOOR <= from_state && from_state <= STATE_IKE_ROOF);
	smc = ike_microcode_index[from_state - STATE_IKE_FLOOR];

	if (st != NULL) {
		oakley_auth_t baseauth =
			xauth_calcbaseauth(st->st_oakley.auth);

		while (!LHAS(smc->flags, baseauth)) {
			smc++;
			passert(smc->state == from_state);
		}
	}

	if (state_busy(st))
		return;

	 
	if (st != NULL &&
	    st->st_rpacket.ptr != NULL &&
	    st->st_rpacket.len == pbs_room(&md->packet_pbs) &&
	    memeq(st->st_rpacket.ptr, md->packet_pbs.start,
		   st->st_rpacket.len)) {
		if (smc->flags & SMF_RETRANSMIT_ON_DUPLICATE) {
			if (st->st_retransmit < MAXIMUM_v1_ACCEPTED_DUPLICATES) {
				st->st_retransmit++;
				loglog(RC_RETRANSMISSION,
				       "retransmitting in response to duplicate packet; already %s",
				       enum_name(&state_names, st->st_state));
				resend_ike_v1_msg(st,
						  "retransmit in response to duplicate");
			} else {
				loglog(RC_LOG_SERIOUS,
				       "discarding duplicate packet -- exhausted retransmission; already %s",
				       enum_name(&state_names, st->st_state));
			}
		} else {
			loglog(RC_LOG_SERIOUS,
			       "discarding duplicate packet; already %s",
			       enum_name(&state_names, st->st_state));
		}
		return;
	}

	 
	md->st = st;
	md->from_state = from_state;
	md->smc = smc;
	md->new_iv_set = new_iv_set;

	 
	if ((md->hdr.isa_flags & ISAKMP_FLAGS_v1_ENCRYPTION) &&
	    st != NULL && !st->hidden_variables.st_skeyid_calculated ) {
		DBG(DBG_CRYPT | DBG_CONTROL, {
			ipstr_buf b;
			DBG_log("received encrypted packet from %s:%u but exponentiation still in progress",
				ipstr(&md->sender, &b),
				(unsigned)md->sender_port);
		});

		 
		if (st->st_suspended_md != NULL) {
			DBG(DBG_CONTROL,
			    DBG_log("releasing suspended operation before completion: %p",
				    st->st_suspended_md));
			release_any_md(&st->st_suspended_md);
		}

		set_suspended(st, md);
		*mdp = NULL;
		return;
	}

	process_packet_tail(mdp);
	 
}
