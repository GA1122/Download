rend_service_rendezvous_has_opened(origin_circuit_t *circuit)
{
  rend_service_t *service;
  char buf[RELAY_PAYLOAD_SIZE];
  crypt_path_t *hop;
  char serviceid[REND_SERVICE_ID_LEN_BASE32+1];
  char hexcookie[9];
  int reason;
  const char *rend_cookie, *rend_pk_digest;

  tor_assert(circuit->base_.purpose == CIRCUIT_PURPOSE_S_CONNECT_REND);
  tor_assert(circuit->cpath);
  tor_assert(circuit->build_state);
  assert_circ_anonymity_ok(circuit, get_options());
  tor_assert(circuit->rend_data);

   
  rend_pk_digest = (char *) rend_data_get_pk_digest(circuit->rend_data,
                                                    NULL);
  rend_cookie = circuit->rend_data->rend_cookie;

   
  if (!circuit->base_.timestamp_dirty)
    circuit->base_.timestamp_dirty = time(NULL);

   
  pathbias_count_use_attempt(circuit);

  hop = circuit->build_state->service_pending_final_cpath_ref->cpath;

  base16_encode(hexcookie,9, rend_cookie,4);
  base32_encode(serviceid, REND_SERVICE_ID_LEN_BASE32+1,
                rend_pk_digest, REND_SERVICE_ID_LEN);

  log_info(LD_REND,
           "Done building circuit %u to rendezvous with "
           "cookie %s for service %s",
           (unsigned)circuit->base_.n_circ_id, hexcookie, serviceid);
  circuit_log_path(LOG_INFO, LD_REND, circuit);

   
  circuit->hs_circ_has_timed_out = 0;

   
  if (hop == NULL) {
    log_info(LD_REND, "Another rend circ has already reached this rend point; "
             "closing this rend circ.");
    reason = END_CIRC_REASON_NONE;
    goto err;
  }

   
  circuit->build_state->pending_final_cpath = hop;
  circuit->build_state->service_pending_final_cpath_ref->cpath = NULL;

  service = rend_service_get_by_pk_digest(rend_pk_digest);
  if (!service) {
    log_warn(LD_GENERAL, "Internal error: unrecognized service ID on "
             "rendezvous circuit.");
    reason = END_CIRC_REASON_INTERNAL;
    goto err;
  }

   
  memcpy(buf, rend_cookie, REND_COOKIE_LEN);
  if (crypto_dh_get_public(hop->rend_dh_handshake_state,
                           buf+REND_COOKIE_LEN, DH_KEY_LEN)<0) {
    log_warn(LD_GENERAL,"Couldn't get DH public key.");
    reason = END_CIRC_REASON_INTERNAL;
    goto err;
  }
  memcpy(buf+REND_COOKIE_LEN+DH_KEY_LEN, hop->rend_circ_nonce,
         DIGEST_LEN);

   
  if (relay_send_command_from_edge(0, TO_CIRCUIT(circuit),
                                   RELAY_COMMAND_RENDEZVOUS1,
                                   buf, REND_COOKIE_LEN+DH_KEY_LEN+DIGEST_LEN,
                                   circuit->cpath->prev)<0) {
    log_warn(LD_GENERAL, "Couldn't send RENDEZVOUS1 cell.");
    goto done;
  }

  crypto_dh_free(hop->rend_dh_handshake_state);
  hop->rend_dh_handshake_state = NULL;

   
  hop->state = CPATH_STATE_OPEN;
   
  hop->package_window = circuit_initial_package_window();
  hop->deliver_window = CIRCWINDOW_START;

  onion_append_to_cpath(&circuit->cpath, hop);
  circuit->build_state->pending_final_cpath = NULL;  

   
  circuit_change_purpose(TO_CIRCUIT(circuit), CIRCUIT_PURPOSE_S_REND_JOINED);

  goto done;

 err:
  circuit_mark_for_close(TO_CIRCUIT(circuit), reason);
 done:
  memwipe(buf, 0, sizeof(buf));
  memwipe(serviceid, 0, sizeof(serviceid));
  memwipe(hexcookie, 0, sizeof(hexcookie));

  return;
}