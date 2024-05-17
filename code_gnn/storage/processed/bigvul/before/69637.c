rend_service_receive_introduction(origin_circuit_t *circuit,
                                  const uint8_t *request,
                                  size_t request_len)
{
   
  int status = 0, result;
  const or_options_t *options = get_options();
  char *err_msg = NULL;
  int err_msg_severity = LOG_WARN;
  const char *stage_descr = NULL, *rend_pk_digest;
  int reason = END_CIRC_REASON_TORPROTOCOL;
   
  char serviceid[REND_SERVICE_ID_LEN_BASE32+1];
  rend_service_t *service = NULL;
  rend_intro_point_t *intro_point = NULL;
  crypto_pk_t *intro_key = NULL;
   
  rend_intro_cell_t *parsed_req = NULL;
   
  extend_info_t *rp = NULL;
   
  char buf[RELAY_PAYLOAD_SIZE];
  char keys[DIGEST_LEN+CPATH_KEY_MATERIAL_LEN];  
  int i;
  crypto_dh_t *dh = NULL;
  origin_circuit_t *launched = NULL;
  crypt_path_t *cpath = NULL;
  char hexcookie[9];
  int circ_needs_uptime;
  time_t now = time(NULL);
  time_t elapsed;
  int replay;

   
  if (circuit->base_.purpose != CIRCUIT_PURPOSE_S_INTRO) {
    log_warn(LD_PROTOCOL,
             "Got an INTRODUCE2 over a non-introduction circuit %u.",
             (unsigned) circuit->base_.n_circ_id);
    goto err;
  }

  assert_circ_anonymity_ok(circuit, options);
  tor_assert(circuit->rend_data);
   
  rend_pk_digest = (char *) rend_data_get_pk_digest(circuit->rend_data, NULL);

   
  base32_encode(serviceid, REND_SERVICE_ID_LEN_BASE32+1,
                rend_pk_digest, REND_SERVICE_ID_LEN);

   
  service = rend_service_get_by_pk_digest(rend_pk_digest);
  if (!service) {
    log_warn(LD_BUG,
             "Internal error: Got an INTRODUCE2 cell on an intro "
             "circ for an unrecognized service %s.",
             escaped(serviceid));
    goto err;
  }

  intro_point = find_intro_point(circuit);
  if (intro_point == NULL) {
    intro_point = find_expiring_intro_point(service, circuit);
    if (intro_point == NULL) {
      log_warn(LD_BUG,
               "Internal error: Got an INTRODUCE2 cell on an "
               "intro circ (for service %s) with no corresponding "
               "rend_intro_point_t.",
               escaped(serviceid));
      goto err;
    }
  }

  log_info(LD_REND, "Received INTRODUCE2 cell for service %s on circ %u.",
           escaped(serviceid), (unsigned)circuit->base_.n_circ_id);

   
  intro_key = circuit->intro_key;

  tor_free(err_msg);
  stage_descr = NULL;

  stage_descr = "early parsing";
   
  parsed_req =
    rend_service_begin_parse_intro(request, request_len, 2, &err_msg);
  if (!parsed_req) {
    goto log_error;
  } else if (err_msg) {
    log_info(LD_REND, "%s on circ %u.", err_msg,
             (unsigned)circuit->base_.n_circ_id);
    tor_free(err_msg);
  }

   
  if (!service->accepted_intro_dh_parts) {
    service->accepted_intro_dh_parts =
      replaycache_new(REND_REPLAY_TIME_INTERVAL,
                      REND_REPLAY_TIME_INTERVAL);
  }

  if (!intro_point->accepted_intro_rsa_parts) {
    intro_point->accepted_intro_rsa_parts = replaycache_new(0, 0);
  }

   
  replay = replaycache_add_test_and_elapsed(
    intro_point->accepted_intro_rsa_parts,
    parsed_req->ciphertext, parsed_req->ciphertext_len,
    &elapsed);

  if (replay) {
    log_warn(LD_REND,
             "Possible replay detected! We received an "
             "INTRODUCE2 cell with same PK-encrypted part %d "
             "seconds ago.  Dropping cell.",
             (int)elapsed);
    goto err;
  }

  stage_descr = "decryption";
   
  result = rend_service_decrypt_intro(parsed_req, intro_key, &err_msg);
  if (result < 0) {
    goto log_error;
  } else if (err_msg) {
    log_info(LD_REND, "%s on circ %u.", err_msg,
             (unsigned)circuit->base_.n_circ_id);
    tor_free(err_msg);
  }

  stage_descr = "late parsing";
   
  result = rend_service_parse_intro_plaintext(parsed_req, &err_msg);
  if (result < 0) {
    goto log_error;
  } else if (err_msg) {
    log_info(LD_REND, "%s on circ %u.", err_msg,
             (unsigned)circuit->base_.n_circ_id);
    tor_free(err_msg);
  }

  stage_descr = "late validation";
   
  result = rend_service_validate_intro_late(parsed_req, &err_msg);
  if (result < 0) {
    goto log_error;
  } else if (err_msg) {
    log_info(LD_REND, "%s on circ %u.", err_msg,
             (unsigned)circuit->base_.n_circ_id);
    tor_free(err_msg);
  }
  stage_descr = NULL;

   
  ++(intro_point->accepted_introduce2_count);

   
  rp = find_rp_for_intro(parsed_req, &err_msg);
  if (!rp) {
    err_msg_severity = LOG_PROTOCOL_WARN;
    goto log_error;
  }

   
  if (options->StrictNodes &&
      routerset_contains_extendinfo(options->ExcludeNodes, rp)) {
    log_warn(LD_REND, "Client asked to rendezvous at a relay that we "
             "exclude, and StrictNodes is set. Refusing service.");
    reason = END_CIRC_REASON_INTERNAL;  
    goto err;
  }

  base16_encode(hexcookie, 9, (const char *)(parsed_req->rc), 4);

   
  replay = replaycache_add_test_and_elapsed(
      service->accepted_intro_dh_parts,
      parsed_req->dh, DH_KEY_LEN,
      &elapsed);

  if (replay) {
     
    log_info(LD_REND, "We received an "
             "INTRODUCE2 cell with same first part of "
             "Diffie-Hellman handshake %d seconds ago. Dropping "
             "cell.",
             (int) elapsed);
    goto err;
  }

   
  if (service->clients) {
    if (parsed_req->version == 3 && parsed_req->u.v3.auth_len > 0) {
      if (rend_check_authorization(service,
                                   (const char*)parsed_req->u.v3.auth_data,
                                   parsed_req->u.v3.auth_len)) {
        log_info(LD_REND, "Authorization data in INTRODUCE2 cell are valid.");
      } else {
        log_info(LD_REND, "The authorization data that are contained in "
                 "the INTRODUCE2 cell are invalid. Dropping cell.");
        reason = END_CIRC_REASON_CONNECTFAILED;
        goto err;
      }
    } else {
      log_info(LD_REND, "INTRODUCE2 cell does not contain authentication "
               "data, but we require client authorization. Dropping cell.");
      reason = END_CIRC_REASON_CONNECTFAILED;
      goto err;
    }
  }

   
  dh = crypto_dh_new(DH_TYPE_REND);
  if (!dh || crypto_dh_generate_public(dh)<0) {
    log_warn(LD_BUG,"Internal error: couldn't build DH state "
             "or generate public key.");
    reason = END_CIRC_REASON_INTERNAL;
    goto err;
  }
  if (crypto_dh_compute_secret(LOG_PROTOCOL_WARN, dh,
                               (char *)(parsed_req->dh),
                               DH_KEY_LEN, keys,
                               DIGEST_LEN+CPATH_KEY_MATERIAL_LEN)<0) {
    log_warn(LD_BUG, "Internal error: couldn't complete DH handshake");
    reason = END_CIRC_REASON_INTERNAL;
    goto err;
  }

  circ_needs_uptime = rend_service_requires_uptime(service);

   
  rep_hist_note_used_internal(now, circ_needs_uptime, 1);

   
  for (i=0;i<MAX_REND_FAILURES;i++) {
    int flags = CIRCLAUNCH_NEED_CAPACITY | CIRCLAUNCH_IS_INTERNAL;
    if (circ_needs_uptime) flags |= CIRCLAUNCH_NEED_UPTIME;
     
    if (rend_service_use_direct_connection(options, rp)) {
      flags = flags | CIRCLAUNCH_ONEHOP_TUNNEL;
    }
    launched = circuit_launch_by_extend_info(
                        CIRCUIT_PURPOSE_S_CONNECT_REND, rp, flags);

    if (launched)
      break;
  }
  if (!launched) {  
    log_warn(LD_REND, "Giving up launching first hop of circuit to rendezvous "
             "point %s for service %s.",
             safe_str_client(extend_info_describe(rp)),
             serviceid);
    reason = END_CIRC_REASON_CONNECTFAILED;
    goto err;
  }
  log_info(LD_REND,
           "Accepted intro; launching circuit to %s "
           "(cookie %s) for service %s.",
           safe_str_client(extend_info_describe(rp)),
           hexcookie, serviceid);
  tor_assert(launched->build_state);
   

  launched->rend_data =
    rend_data_service_create(service->service_id, rend_pk_digest,
                             parsed_req->rc, service->auth_type);

  launched->build_state->service_pending_final_cpath_ref =
    tor_malloc_zero(sizeof(crypt_path_reference_t));
  launched->build_state->service_pending_final_cpath_ref->refcount = 1;

  launched->build_state->service_pending_final_cpath_ref->cpath = cpath =
    tor_malloc_zero(sizeof(crypt_path_t));
  cpath->magic = CRYPT_PATH_MAGIC;
  launched->build_state->expiry_time = now + MAX_REND_TIMEOUT;

  cpath->rend_dh_handshake_state = dh;
  dh = NULL;
  if (circuit_init_cpath_crypto(cpath,keys+DIGEST_LEN,1)<0)
    goto err;
  memcpy(cpath->rend_circ_nonce, keys, DIGEST_LEN);

  goto done;

 log_error:
  if (!err_msg) {
    if (stage_descr) {
      tor_asprintf(&err_msg,
                   "unknown %s error for INTRODUCE2", stage_descr);
    } else {
      err_msg = tor_strdup("unknown error for INTRODUCE2");
    }
  }

  log_fn(err_msg_severity, LD_REND, "%s on circ %u", err_msg,
           (unsigned)circuit->base_.n_circ_id);
 err:
  status = -1;
  if (dh) crypto_dh_free(dh);
  if (launched) {
    circuit_mark_for_close(TO_CIRCUIT(launched), reason);
  }
  tor_free(err_msg);

 done:
  memwipe(keys, 0, sizeof(keys));
  memwipe(buf, 0, sizeof(buf));
  memwipe(serviceid, 0, sizeof(serviceid));
  memwipe(hexcookie, 0, sizeof(hexcookie));

   
  rend_service_free_intro(parsed_req);

   
  extend_info_free(rp);

  return status;
}