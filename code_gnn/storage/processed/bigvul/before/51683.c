bool ikev1_decode_peer_id(struct msg_digest *md, bool initiator, bool aggrmode)
{
	struct state *const st = md->st;
	struct payload_digest *const id_pld = md->chain[ISAKMP_NEXT_ID];
	const pb_stream *const id_pbs = &id_pld->pbs;
	struct isakmp_id *const id = &id_pld->payload.id;
	struct id peer;

	 
	if (st->hidden_variables.st_nat_traversal != LEMPTY &&
	    id->isaid_doi_specific_a == IPPROTO_UDP &&
	    (id->isaid_doi_specific_b == 0 ||
	     id->isaid_doi_specific_b == pluto_nat_port)) {
		DBG_log("protocol/port in Phase 1 ID Payload is %d/%d. "
			"accepted with port_floating NAT-T",
			id->isaid_doi_specific_a, id->isaid_doi_specific_b);
	} else if (!(id->isaid_doi_specific_a == 0 &&
		     id->isaid_doi_specific_b == 0) &&
		   !(id->isaid_doi_specific_a == IPPROTO_UDP &&
		     id->isaid_doi_specific_b == pluto_port))
	{
		loglog(RC_LOG_SERIOUS, "protocol/port in Phase 1 ID Payload MUST be 0/0 or %d/%d"
		       " but are %d/%d (attempting to continue)",
		       IPPROTO_UDP, pluto_port,
		       id->isaid_doi_specific_a,
		       id->isaid_doi_specific_b);
		 
		 
	}

	zero(&peer);	 
	peer.kind = id->isaid_idtype;

	if (!extract_peer_id(&peer, id_pbs))
		return FALSE;

	 
	st->st_peeridentity_protocol = id->isaid_doi_specific_a;
	st->st_peeridentity_port = ntohs(id->isaid_doi_specific_b);

	{
		char buf[IDTOA_BUF];

		idtoa(&peer, buf, sizeof(buf));
		libreswan_log("%s mode peer ID is %s: '%s'",
			      aggrmode ? "Aggressive" : "Main",
			      enum_show(&ike_idtype_names, id->isaid_idtype), buf);
	}

	 
	if (!ikev1_decode_cert(md))
		return FALSE;

	 
	if (initiator) {
		if (!same_id(&st->st_connection->spd.that.id, &peer) &&
		     id_kind(&st->st_connection->spd.that.id) != ID_FROMCERT) {
			char expect[IDTOA_BUF],
			     found[IDTOA_BUF];

			idtoa(&st->st_connection->spd.that.id, expect,
			      sizeof(expect));
			idtoa(&peer, found, sizeof(found));
			loglog(RC_LOG_SERIOUS,
			       "we require IKEv1 peer to have ID '%s', but peer declares '%s'",
			       expect, found);
			return FALSE;
		} else if (id_kind(&st->st_connection->spd.that.id) == ID_FROMCERT) {
			if (id_kind(&peer) != ID_DER_ASN1_DN) {
				loglog(RC_LOG_SERIOUS,
				       "peer ID is not a certificate type");
				return FALSE;
			}
			duplicate_id(&st->st_connection->spd.that.id, &peer);
		}
	} else {
		struct connection *c = st->st_connection;
		struct connection *r = NULL;
		bool fromcert;
		uint16_t auth = xauth_calcbaseauth(st->st_oakley.auth);
		lset_t auth_policy = LEMPTY;

		switch (auth) {
		case OAKLEY_PRESHARED_KEY:
			auth_policy = POLICY_PSK;
			break;
		case OAKLEY_RSA_SIG:
			auth_policy = POLICY_RSASIG;
			break;
		 
		case OAKLEY_DSS_SIG:
		case OAKLEY_RSA_ENC:
		case OAKLEY_RSA_REVISED_MODE:
		case OAKLEY_ECDSA_P256:
		case OAKLEY_ECDSA_P384:
		case OAKLEY_ECDSA_P521:
		default:
			DBG(DBG_CONTROL, DBG_log("ikev1 ikev1_decode_peer_id bad_case due to not supported policy"));
		}

		if (aggrmode)
			auth_policy |=  POLICY_AGGRESSIVE;

		 
		ikev1_decode_cr(md);

		if ((auth_policy & ~POLICY_AGGRESSIVE) != LEMPTY) {
			r = refine_host_connection(st, &peer, initiator, auth_policy, &fromcert);
			pexpect(r != NULL);
		}

		if (r == NULL) {
			char buf[IDTOA_BUF];

			idtoa(&peer, buf, sizeof(buf));
			loglog(RC_LOG_SERIOUS,
			       "no suitable connection for peer '%s'",
			       buf);
			return FALSE;
		}

		DBG(DBG_CONTROL, {
			    char buf[IDTOA_BUF];
			    dntoa_or_null(buf, IDTOA_BUF, r->spd.this.ca,
					  "%none");
			    DBG_log("offered CA: '%s'", buf);
		    });

		if (r != c) {
			char b1[CONN_INST_BUF];
			char b2[CONN_INST_BUF];

			 
			libreswan_log("switched from \"%s\"%s to \"%s\"%s",
				c->name,
				fmt_conn_instance(c, b1),
				r->name,
				fmt_conn_instance(r, b2));
			if (r->kind == CK_TEMPLATE || r->kind == CK_GROUP) {
				 
				r = rw_instantiate(r, &c->spd.that.host_addr,
						   NULL,
						   &peer);
			}

			update_state_connection(st, r);
		} else if (c->spd.that.has_id_wildcards) {
			free_id_content(&c->spd.that.id);
			c->spd.that.id = peer;
			c->spd.that.has_id_wildcards = FALSE;
			unshare_id_content(&c->spd.that.id);
		} else if (fromcert) {
			DBG(DBG_CONTROL, DBG_log("copying ID for fromcert"));
			duplicate_id(&r->spd.that.id, &peer);
		}
	}

	return TRUE;
}
