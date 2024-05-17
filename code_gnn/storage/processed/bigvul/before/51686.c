static stf_status informational(struct msg_digest *md)
{
	struct payload_digest *const n_pld = md->chain[ISAKMP_NEXT_N];

	 
	if (n_pld != NULL) {
		pb_stream *const n_pbs = &n_pld->pbs;
		struct isakmp_notification *const n =
			&n_pld->payload.notification;
		struct state *st = md->st;     

		 
		 
		DBG(DBG_CONTROL, DBG_log("processing informational %s (%d)",
					 enum_name(&ikev1_notify_names,
						   n->isan_type),
					 n->isan_type));

		switch (n->isan_type) {
		case R_U_THERE:
			if (st == NULL) {
				loglog(RC_LOG_SERIOUS,
				       "received bogus  R_U_THERE informational message");
				return STF_IGNORE;
			}
			return dpd_inI_outR(st, n, n_pbs);

		case R_U_THERE_ACK:
			if (st == NULL) {
				loglog(RC_LOG_SERIOUS,
				       "received bogus R_U_THERE_ACK informational message");
				return STF_IGNORE;
			}
			return dpd_inR(st, n, n_pbs);

		case PAYLOAD_MALFORMED:
			if (st != NULL) {
				st->hidden_variables.st_malformed_received++;

				libreswan_log(
					"received %u malformed payload notifies",
					st->hidden_variables.st_malformed_received);

				if (st->hidden_variables.st_malformed_sent >
				    MAXIMUM_MALFORMED_NOTIFY / 2 &&
				    ((st->hidden_variables.st_malformed_sent +
				      st->hidden_variables.
				      st_malformed_received) >
				     MAXIMUM_MALFORMED_NOTIFY)) {
					libreswan_log(
						"too many malformed payloads (we sent %u and received %u",
						st->hidden_variables.st_malformed_sent,
						st->hidden_variables.st_malformed_received);
					delete_state(st);
					md->st = st = NULL;
				}
			}
			return STF_IGNORE;

		case ISAKMP_N_CISCO_LOAD_BALANCE:
			if (st != NULL && IS_ISAKMP_SA_ESTABLISHED(st->st_state)) {
				 
				const char *tmp_name = st->st_connection->name;
				int tmp_whack_sock = dup_any(st->st_whack_sock);

				 
				delete_state(st);
				md->st = st = NULL;

				 
				 
				struct connection *tmp_c = con_by_name(tmp_name, FALSE);

				DBG_cond_dump(DBG_PARSING,
					      "redirected remote end info:", n_pbs->cur + pbs_left(
						      n_pbs) - 4, 4);

				 
				{

					ipstr_buf b;
					const struct spd_route *tmp_spd =
						&tmp_c->spd;
					int count_spd = 0;

					do {
						DBG(DBG_CONTROLMORE,
						    DBG_log("spd route number: %d",
							    ++count_spd));

						 
						DBG(DBG_CONTROLMORE,
						    DBG_log("that id kind: %d",
							    tmp_spd->that.id.kind));
						DBG(DBG_CONTROLMORE,
						    DBG_log("that id ipaddr: %s",
							    ipstr(&tmp_spd->that.id.ip_addr, &b)));
						if (tmp_spd->that.id.name.ptr
						    != NULL)
							DBG(DBG_CONTROLMORE,
							    DBG_dump_chunk(
								    "that id name",
								    tmp_spd->
								    that.id.
								    name));
						DBG(DBG_CONTROLMORE,
						    DBG_log("that host_addr: %s",
							    ipstr(&tmp_spd->that.host_addr, &b)));
						DBG(DBG_CONTROLMORE,
						    DBG_log("that nexthop: %s",
							    ipstr(&tmp_spd->that.host_nexthop, &b)));
						DBG(DBG_CONTROLMORE,
						    DBG_log("that srcip: %s",
							    ipstr(&tmp_spd->that.host_srcip, &b)));
						DBG(DBG_CONTROLMORE,
						    DBG_log("that client_addr: %s, maskbits:%d",
							    ipstr(&tmp_spd->that.client.addr, &b),
							    tmp_spd->that.
							    client.maskbits));
						DBG(DBG_CONTROLMORE,
						    DBG_log("that has_client: %d",
							    tmp_spd->that.
							    has_client));
						DBG(DBG_CONTROLMORE,
						    DBG_log("that has_client_wildcard: %d",
							    tmp_spd->that.
							    has_client_wildcard));
						DBG(DBG_CONTROLMORE,
						    DBG_log("that has_port_wildcard: %d",
							    tmp_spd->that.
							    has_port_wildcard));
						DBG(DBG_CONTROLMORE,
						    DBG_log("that has_id_wildcards: %d",
							    tmp_spd->that.
							    has_id_wildcards));

						tmp_spd = tmp_spd->spd_next;
					} while (tmp_spd != NULL);

					if (tmp_c->interface != NULL) {
						DBG(DBG_CONTROLMORE,
						    DBG_log("Current interface_addr: %s",
							    ipstr(&tmp_c->interface->ip_addr, &b)));
					}

					if (tmp_c->gw_info != NULL) {
						DBG(DBG_CONTROLMORE, {
							    DBG_log("Current gw_client_addr: %s",
								    ipstr(&tmp_c->gw_info->client_id.ip_addr, &b));
							    DBG_log("Current gw_gw_addr: %s",
								    ipstr(&tmp_c->gw_info->gw_id.ip_addr, &b));
						    });
					}

				}

				 
				ip_address old_addr = tmp_c->spd.that.host_addr;

				 
				memcpy(&tmp_c->spd.that.host_addr.u.v4.sin_addr.s_addr,
					(u_int32_t *)(n_pbs->cur +
						      pbs_left(n_pbs) - 4),
					sizeof(tmp_c->spd.that.host_addr.u.v4.
					       sin_addr.
					       s_addr));

				 
				DBG(DBG_CONTROLMORE,
				    DBG_log("Old host_addr_name : %s",
					    tmp_c->spd.that.host_addr_name));
				tmp_c->spd.that.host_addr_name = NULL;
				tmp_c->spd.that.id.ip_addr =
					tmp_c->spd.that.host_addr;

				DBG(DBG_CONTROLMORE, {
					ipstr_buf b;
					if (sameaddr(&tmp_c->spd.this.
						     host_nexthop,
						     &old_addr)) {
						DBG_log("Old remote addr %s",
							ipstr(&old_addr, &b));
						DBG_log("Old this host next hop %s",
							ipstr(&tmp_c->spd.this.host_nexthop, &b));
						tmp_c->spd.this.host_nexthop = tmp_c->spd.that.host_addr;
						DBG_log("New this host next hop %s",
							ipstr(&tmp_c->spd.this.host_nexthop, &b));
					}

					if (sameaddr(&tmp_c->spd.that.
						     host_srcip,
						     &old_addr)) {
						DBG_log("Old that host srcip %s",
							ipstr(&tmp_c->spd.that.host_srcip, &b));
						tmp_c->spd.that.host_srcip = tmp_c->spd.that.host_addr;
						DBG_log("New that host srcip %s",
							ipstr(&tmp_c->spd.that.host_srcip, &b));
					}

					if (sameaddr(&tmp_c->spd.that.
						     client.addr,
						     &old_addr)) {
						DBG_log("Old that client ip %s",
							ipstr(&tmp_c->spd.that.client.addr, &b));
						tmp_c->spd.that.client.addr = tmp_c->spd.that.host_addr;
						DBG_log("New that client ip %s",
							ipstr(&tmp_c->spd.that.client.addr, &b));
					}
				});

				tmp_c->host_pair->him.addr =
					tmp_c->spd.that.host_addr;

				 
				initiate_connection(tmp_name, tmp_whack_sock,
						    LEMPTY, pcim_demand_crypto);
				return STF_IGNORE;
			}
			loglog(RC_LOG_SERIOUS,
			       "received and ignored informational message with ISAKMP_N_CISCO_LOAD_BALANCE for unestablished state.");
			return STF_IGNORE;
		default:
			if (st != NULL &&
			    (st->st_connection->extra_debugging &
			     IMPAIR_DIE_ONINFO)) {
				loglog(RC_LOG_SERIOUS,
				       "received unhandled informational notification payload %d: '%s'",
				       n->isan_type,
				       enum_name(&ikev1_notify_names,
						 n->isan_type));
				return STF_FATAL;
			}
			loglog(RC_LOG_SERIOUS,
			       "received and ignored informational message");
			return STF_IGNORE;
		}
	} else {
		loglog(RC_LOG_SERIOUS,
		       "received and ignored empty informational notification payload");
		return STF_IGNORE;
	}
}