rend_service_intro_has_opened(origin_circuit_t *circuit)
{
  rend_service_t *service;
  char buf[RELAY_PAYLOAD_SIZE];
  char serviceid[REND_SERVICE_ID_LEN_BASE32+1];
  unsigned int expiring_nodes_len, num_ip_circuits, valid_ip_circuits = 0;
  int reason = END_CIRC_REASON_TORPROTOCOL;
  const char *rend_pk_digest;

  tor_assert(circuit->base_.purpose == CIRCUIT_PURPOSE_S_ESTABLISH_INTRO);
  assert_circ_anonymity_ok(circuit, get_options());
  tor_assert(circuit->cpath);
  tor_assert(circuit->rend_data);
   
  rend_pk_digest = (char *) rend_data_get_pk_digest(circuit->rend_data, NULL);

  base32_encode(serviceid, REND_SERVICE_ID_LEN_BASE32+1,
                rend_pk_digest, REND_SERVICE_ID_LEN);

  service = rend_service_get_by_pk_digest(rend_pk_digest);
  if (!service) {
    log_warn(LD_REND, "Unrecognized service ID %s on introduction circuit %u.",
             safe_str_client(serviceid), (unsigned)circuit->base_.n_circ_id);
    reason = END_CIRC_REASON_NOSUCHSERVICE;
    goto err;
  }

   
  expiring_nodes_len = (unsigned int) smartlist_len(service->expiring_nodes);
  num_ip_circuits = count_intro_point_circuits(service);
   
  if (num_ip_circuits > expiring_nodes_len) {
    valid_ip_circuits = num_ip_circuits - expiring_nodes_len;
  }

   
  if (valid_ip_circuits > service->n_intro_points_wanted) {
    const or_options_t *options = get_options();
     
    rend_intro_point_t *intro = find_intro_point(circuit);
    if (intro != NULL) {
      smartlist_remove(service->intro_nodes, intro);
      rend_intro_point_free(intro);
    }

    if (options->ExcludeNodes) {
       
      log_info(LD_CIRC|LD_REND, "We have just finished an introduction "
               "circuit, but we already have enough.  Closing it.");
      reason = END_CIRC_REASON_NONE;
      goto err;
    } else {
      tor_assert(circuit->build_state->is_internal);
      log_info(LD_CIRC|LD_REND, "We have just finished an introduction "
               "circuit, but we already have enough. Redefining purpose to "
               "general; leaving as internal.");

      circuit_change_purpose(TO_CIRCUIT(circuit), CIRCUIT_PURPOSE_C_GENERAL);

      {
        rend_data_free(circuit->rend_data);
        circuit->rend_data = NULL;
      }
      {
        crypto_pk_t *intro_key = circuit->intro_key;
        circuit->intro_key = NULL;
        crypto_pk_free(intro_key);
      }

      circuit_has_opened(circuit);
      goto done;
    }
  }

  log_info(LD_REND,
           "Established circuit %u as introduction point for service %s",
           (unsigned)circuit->base_.n_circ_id, serviceid);
  circuit_log_path(LOG_INFO, LD_REND, circuit);

   
  {
    ssize_t len;
    len = encode_establish_intro_cell_legacy(buf, sizeof(buf),
                                      circuit->intro_key,
                                      circuit->cpath->prev->rend_circ_nonce);
    if (len < 0) {
      reason = END_CIRC_REASON_INTERNAL;
      goto err;
    }

    if (relay_send_command_from_edge(0, TO_CIRCUIT(circuit),
                                     RELAY_COMMAND_ESTABLISH_INTRO,
                                     buf, len, circuit->cpath->prev)<0) {
      log_info(LD_GENERAL,
             "Couldn't send introduction request for service %s on circuit %u",
             serviceid, (unsigned)circuit->base_.n_circ_id);
      goto done;
    }
  }

   
  pathbias_count_use_attempt(circuit);

  goto done;

 err:
  circuit_mark_for_close(TO_CIRCUIT(circuit), reason);
 done:
  memwipe(buf, 0, sizeof(buf));
  memwipe(serviceid, 0, sizeof(serviceid));

  return;
}
