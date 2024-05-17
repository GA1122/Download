static int sctp_cmd_interpreter(sctp_event_t event_type,
				sctp_subtype_t subtype,
				sctp_state_t state,
				struct sctp_endpoint *ep,
				struct sctp_association *asoc,
				void *event_arg,
				sctp_disposition_t status,
				sctp_cmd_seq_t *commands,
				gfp_t gfp)
{
	int error = 0;
	int force;
	sctp_cmd_t *cmd;
	struct sctp_chunk *new_obj;
	struct sctp_chunk *chunk = NULL;
	struct sctp_packet *packet;
	struct timer_list *timer;
	unsigned long timeout;
	struct sctp_transport *t;
	struct sctp_sackhdr sackh;
	int local_cork = 0;

	if (SCTP_EVENT_T_TIMEOUT != event_type)
		chunk = event_arg;

	 
	while (NULL != (cmd = sctp_next_cmd(commands))) {
		switch (cmd->verb) {
		case SCTP_CMD_NOP:
			 
			break;

		case SCTP_CMD_NEW_ASOC:
			 
			if (local_cork) {
				sctp_outq_uncork(&asoc->outqueue);
				local_cork = 0;
			}

			 
			asoc = cmd->obj.asoc;
			BUG_ON(asoc->peer.primary_path == NULL);
			sctp_endpoint_add_asoc(ep, asoc);
			sctp_hash_established(asoc);
			break;

		case SCTP_CMD_UPDATE_ASSOC:
		       sctp_assoc_update(asoc, cmd->obj.asoc);
		       break;

		case SCTP_CMD_PURGE_OUTQUEUE:
		       sctp_outq_teardown(&asoc->outqueue);
		       break;

		case SCTP_CMD_DELETE_TCB:
			if (local_cork) {
				sctp_outq_uncork(&asoc->outqueue);
				local_cork = 0;
			}
			 
			sctp_cmd_delete_tcb(commands, asoc);
			asoc = NULL;
			break;

		case SCTP_CMD_NEW_STATE:
			 
			sctp_cmd_new_state(commands, asoc, cmd->obj.state);
			break;

		case SCTP_CMD_REPORT_TSN:
			 
			error = sctp_tsnmap_mark(&asoc->peer.tsn_map,
						 cmd->obj.u32, NULL);
			break;

		case SCTP_CMD_REPORT_FWDTSN:
			 
			sctp_tsnmap_skip(&asoc->peer.tsn_map, cmd->obj.u32);

			 
			sctp_ulpq_reasm_flushtsn(&asoc->ulpq, cmd->obj.u32);

			 
			sctp_ulpq_abort_pd(&asoc->ulpq, GFP_ATOMIC);
			break;

		case SCTP_CMD_PROCESS_FWDTSN:
			sctp_cmd_process_fwdtsn(&asoc->ulpq, cmd->obj.chunk);
			break;

		case SCTP_CMD_GEN_SACK:
			 
			force = cmd->obj.i32;
			error = sctp_gen_sack(asoc, force, commands);
			break;

		case SCTP_CMD_PROCESS_SACK:
			 
			error = sctp_cmd_process_sack(commands, asoc,
						      cmd->obj.chunk);
			break;

		case SCTP_CMD_GEN_INIT_ACK:
			 
			new_obj = sctp_make_init_ack(asoc, chunk, GFP_ATOMIC,
						     0);
			if (!new_obj)
				goto nomem;

			sctp_add_cmd_sf(commands, SCTP_CMD_REPLY,
					SCTP_CHUNK(new_obj));
			break;

		case SCTP_CMD_PEER_INIT:
			 
			error = sctp_cmd_process_init(commands, asoc, chunk,
						      cmd->obj.init, gfp);
			break;

		case SCTP_CMD_GEN_COOKIE_ECHO:
			 
			new_obj = sctp_make_cookie_echo(asoc, chunk);
			if (!new_obj) {
				if (cmd->obj.chunk)
					sctp_chunk_free(cmd->obj.chunk);
				goto nomem;
			}
			sctp_add_cmd_sf(commands, SCTP_CMD_REPLY,
					SCTP_CHUNK(new_obj));

			 
			if (cmd->obj.chunk)
				sctp_add_cmd_sf(commands, SCTP_CMD_REPLY,
						SCTP_CHUNK(cmd->obj.chunk));

			if (new_obj->transport) {
				new_obj->transport->init_sent_count++;
				asoc->init_last_sent_to = new_obj->transport;
			}

			 
			if ((asoc->peer.retran_path !=
			     asoc->peer.primary_path) &&
			    (asoc->init_err_counter > 0)) {
				sctp_add_cmd_sf(commands,
						SCTP_CMD_FORCE_PRIM_RETRAN,
						SCTP_NULL());
			}

			break;

		case SCTP_CMD_GEN_SHUTDOWN:
			 
			asoc->overall_error_count = 0;

			 
			new_obj = sctp_make_shutdown(asoc, chunk);
			if (!new_obj)
				goto nomem;
			sctp_add_cmd_sf(commands, SCTP_CMD_REPLY,
					SCTP_CHUNK(new_obj));
			break;

		case SCTP_CMD_CHUNK_ULP:
			 
			pr_debug("%s: sm_sideff: chunk_up:%p, ulpq:%p\n",
				 __func__, cmd->obj.chunk, &asoc->ulpq);

			sctp_ulpq_tail_data(&asoc->ulpq, cmd->obj.chunk,
					    GFP_ATOMIC);
			break;

		case SCTP_CMD_EVENT_ULP:
			 
			pr_debug("%s: sm_sideff: event_up:%p, ulpq:%p\n",
				 __func__, cmd->obj.ulpevent, &asoc->ulpq);

			sctp_ulpq_tail_event(&asoc->ulpq, cmd->obj.ulpevent);
			break;

		case SCTP_CMD_REPLY:
			 
			if (!asoc->outqueue.cork) {
				sctp_outq_cork(&asoc->outqueue);
				local_cork = 1;
			}
			 
			error = sctp_outq_tail(&asoc->outqueue, cmd->obj.chunk);
			break;

		case SCTP_CMD_SEND_PKT:
			 
			packet = cmd->obj.packet;
			sctp_packet_transmit(packet);
			sctp_ootb_pkt_free(packet);
			break;

		case SCTP_CMD_T1_RETRAN:
			 
			sctp_retransmit(&asoc->outqueue, cmd->obj.transport,
					SCTP_RTXR_T1_RTX);
			break;

		case SCTP_CMD_RETRAN:
			 
			sctp_retransmit(&asoc->outqueue, cmd->obj.transport,
					SCTP_RTXR_T3_RTX);
			break;

		case SCTP_CMD_ECN_CE:
			 
			sctp_do_ecn_ce_work(asoc, cmd->obj.u32);
			break;

		case SCTP_CMD_ECN_ECNE:
			 
			new_obj = sctp_do_ecn_ecne_work(asoc, cmd->obj.u32,
							chunk);
			if (new_obj)
				sctp_add_cmd_sf(commands, SCTP_CMD_REPLY,
						SCTP_CHUNK(new_obj));
			break;

		case SCTP_CMD_ECN_CWR:
			 
			sctp_do_ecn_cwr_work(asoc, cmd->obj.u32);
			break;

		case SCTP_CMD_SETUP_T2:
			sctp_cmd_setup_t2(commands, asoc, cmd->obj.chunk);
			break;

		case SCTP_CMD_TIMER_START_ONCE:
			timer = &asoc->timers[cmd->obj.to];

			if (timer_pending(timer))
				break;
			 

		case SCTP_CMD_TIMER_START:
			timer = &asoc->timers[cmd->obj.to];
			timeout = asoc->timeouts[cmd->obj.to];
			BUG_ON(!timeout);

			timer->expires = jiffies + timeout;
			sctp_association_hold(asoc);
			add_timer(timer);
			break;

		case SCTP_CMD_TIMER_RESTART:
			timer = &asoc->timers[cmd->obj.to];
			timeout = asoc->timeouts[cmd->obj.to];
			if (!mod_timer(timer, jiffies + timeout))
				sctp_association_hold(asoc);
			break;

		case SCTP_CMD_TIMER_STOP:
			timer = &asoc->timers[cmd->obj.to];
			if (del_timer(timer))
				sctp_association_put(asoc);
			break;

		case SCTP_CMD_INIT_CHOOSE_TRANSPORT:
			chunk = cmd->obj.chunk;
			t = sctp_assoc_choose_alter_transport(asoc,
						asoc->init_last_sent_to);
			asoc->init_last_sent_to = t;
			chunk->transport = t;
			t->init_sent_count++;
			 
			sctp_assoc_set_primary(asoc, t);
			break;

		case SCTP_CMD_INIT_RESTART:
			 
			sctp_cmd_t1_timer_update(asoc,
						SCTP_EVENT_TIMEOUT_T1_INIT,
						"INIT");

			sctp_add_cmd_sf(commands, SCTP_CMD_TIMER_RESTART,
					SCTP_TO(SCTP_EVENT_TIMEOUT_T1_INIT));
			break;

		case SCTP_CMD_COOKIEECHO_RESTART:
			 
			sctp_cmd_t1_timer_update(asoc,
						SCTP_EVENT_TIMEOUT_T1_COOKIE,
						"COOKIE");

			 
			list_for_each_entry(t, &asoc->peer.transport_addr_list,
					transports) {
				sctp_retransmit_mark(&asoc->outqueue, t,
					    SCTP_RTXR_T1_RTX);
			}

			sctp_add_cmd_sf(commands,
					SCTP_CMD_TIMER_RESTART,
					SCTP_TO(SCTP_EVENT_TIMEOUT_T1_COOKIE));
			break;

		case SCTP_CMD_INIT_FAILED:
			sctp_cmd_init_failed(commands, asoc, cmd->obj.err);
			break;

		case SCTP_CMD_ASSOC_FAILED:
			sctp_cmd_assoc_failed(commands, asoc, event_type,
					      subtype, chunk, cmd->obj.err);
			break;

		case SCTP_CMD_INIT_COUNTER_INC:
			asoc->init_err_counter++;
			break;

		case SCTP_CMD_INIT_COUNTER_RESET:
			asoc->init_err_counter = 0;
			asoc->init_cycle = 0;
			list_for_each_entry(t, &asoc->peer.transport_addr_list,
					    transports) {
				t->init_sent_count = 0;
			}
			break;

		case SCTP_CMD_REPORT_DUP:
			sctp_tsnmap_mark_dup(&asoc->peer.tsn_map,
					     cmd->obj.u32);
			break;

		case SCTP_CMD_REPORT_BAD_TAG:
			pr_debug("%s: vtag mismatch!\n", __func__);
			break;

		case SCTP_CMD_STRIKE:
			 
			sctp_do_8_2_transport_strike(commands, asoc,
						    cmd->obj.transport, 0);
			break;

		case SCTP_CMD_TRANSPORT_IDLE:
			t = cmd->obj.transport;
			sctp_transport_lower_cwnd(t, SCTP_LOWER_CWND_INACTIVE);
			break;

		case SCTP_CMD_TRANSPORT_HB_SENT:
			t = cmd->obj.transport;
			sctp_do_8_2_transport_strike(commands, asoc,
						     t, 1);
			t->hb_sent = 1;
			break;

		case SCTP_CMD_TRANSPORT_ON:
			t = cmd->obj.transport;
			sctp_cmd_transport_on(commands, asoc, t, chunk);
			break;

		case SCTP_CMD_HB_TIMERS_START:
			sctp_cmd_hb_timers_start(commands, asoc);
			break;

		case SCTP_CMD_HB_TIMER_UPDATE:
			t = cmd->obj.transport;
			sctp_cmd_hb_timer_update(commands, t);
			break;

		case SCTP_CMD_HB_TIMERS_STOP:
			sctp_cmd_hb_timers_stop(commands, asoc);
			break;

		case SCTP_CMD_REPORT_ERROR:
			error = cmd->obj.error;
			break;

		case SCTP_CMD_PROCESS_CTSN:
			 
			sackh.cum_tsn_ack = cmd->obj.be32;
			sackh.a_rwnd = asoc->peer.rwnd +
					asoc->outqueue.outstanding_bytes;
			sackh.num_gap_ack_blocks = 0;
			sackh.num_dup_tsns = 0;
			chunk->subh.sack_hdr = &sackh;
			sctp_add_cmd_sf(commands, SCTP_CMD_PROCESS_SACK,
					SCTP_CHUNK(chunk));
			break;

		case SCTP_CMD_DISCARD_PACKET:
			 
			chunk->pdiscard = 1;
			if (asoc) {
				sctp_outq_uncork(&asoc->outqueue);
				local_cork = 0;
			}
			break;

		case SCTP_CMD_RTO_PENDING:
			t = cmd->obj.transport;
			t->rto_pending = 1;
			break;

		case SCTP_CMD_PART_DELIVER:
			sctp_ulpq_partial_delivery(&asoc->ulpq, GFP_ATOMIC);
			break;

		case SCTP_CMD_RENEGE:
			sctp_ulpq_renege(&asoc->ulpq, cmd->obj.chunk,
					 GFP_ATOMIC);
			break;

		case SCTP_CMD_SETUP_T4:
			sctp_cmd_setup_t4(commands, asoc, cmd->obj.chunk);
			break;

		case SCTP_CMD_PROCESS_OPERR:
			sctp_cmd_process_operr(commands, asoc, chunk);
			break;
		case SCTP_CMD_CLEAR_INIT_TAG:
			asoc->peer.i.init_tag = 0;
			break;
		case SCTP_CMD_DEL_NON_PRIMARY:
			sctp_cmd_del_non_primary(asoc);
			break;
		case SCTP_CMD_T3_RTX_TIMERS_STOP:
			sctp_cmd_t3_rtx_timers_stop(commands, asoc);
			break;
		case SCTP_CMD_FORCE_PRIM_RETRAN:
			t = asoc->peer.retran_path;
			asoc->peer.retran_path = asoc->peer.primary_path;
			error = sctp_outq_uncork(&asoc->outqueue);
			local_cork = 0;
			asoc->peer.retran_path = t;
			break;
		case SCTP_CMD_SET_SK_ERR:
			sctp_cmd_set_sk_err(asoc, cmd->obj.error);
			break;
		case SCTP_CMD_ASSOC_CHANGE:
			sctp_cmd_assoc_change(commands, asoc,
					      cmd->obj.u8);
			break;
		case SCTP_CMD_ADAPTATION_IND:
			sctp_cmd_adaptation_ind(commands, asoc);
			break;

		case SCTP_CMD_ASSOC_SHKEY:
			error = sctp_auth_asoc_init_active_key(asoc,
						GFP_ATOMIC);
			break;
		case SCTP_CMD_UPDATE_INITTAG:
			asoc->peer.i.init_tag = cmd->obj.u32;
			break;
		case SCTP_CMD_SEND_MSG:
			if (!asoc->outqueue.cork) {
				sctp_outq_cork(&asoc->outqueue);
				local_cork = 1;
			}
			error = sctp_cmd_send_msg(asoc, cmd->obj.msg);
			break;
		case SCTP_CMD_SEND_NEXT_ASCONF:
			sctp_cmd_send_asconf(asoc);
			break;
		case SCTP_CMD_PURGE_ASCONF_QUEUE:
			sctp_asconf_queue_teardown(asoc);
			break;

		case SCTP_CMD_SET_ASOC:
			asoc = cmd->obj.asoc;
			break;

		default:
			pr_warn("Impossible command: %u\n",
				cmd->verb);
			break;
		}

		if (error)
			break;
	}

out:
	 
	if (asoc && SCTP_EVENT_T_CHUNK == event_type && chunk) {
		if (chunk->end_of_packet || chunk->singleton)
			error = sctp_outq_uncork(&asoc->outqueue);
	} else if (local_cork)
		error = sctp_outq_uncork(&asoc->outqueue);
	return error;
nomem:
	error = -ENOMEM;
	goto out;
}