rend_service_launch_establish_intro(rend_service_t *service,
                                    rend_intro_point_t *intro)
{
  origin_circuit_t *launched;
  int flags = CIRCLAUNCH_NEED_UPTIME|CIRCLAUNCH_IS_INTERNAL;
  const or_options_t *options = get_options();
  extend_info_t *launch_ei = intro->extend_info;
  extend_info_t *direct_ei = NULL;

   
  if (rend_service_allow_non_anonymous_connection(options)) {
     
    const node_t *node = node_get_by_id(launch_ei->identity_digest);
    if (BUG(!node)) {
       
      return -1;
    }
     
    if (rend_service_use_direct_connection_node(options, node)) {
      direct_ei = extend_info_from_node(node, 1);
      if (BUG(!direct_ei)) {
         
        return -1;
      }
      flags = flags | CIRCLAUNCH_ONEHOP_TUNNEL;
      launch_ei = direct_ei;
    }
  }
   
  tor_assert(launch_ei);
   
  tor_assert(tor_memeq(intro->extend_info->identity_digest,
                       launch_ei->identity_digest,
                       DIGEST_LEN));

  log_info(LD_REND,
           "Launching circuit to introduction point %s%s%s for service %s",
           safe_str_client(extend_info_describe(intro->extend_info)),
           direct_ei ? " via direct address " : "",
           direct_ei ? safe_str_client(extend_info_describe(direct_ei)) : "",
           service->service_id);

  rep_hist_note_used_internal(time(NULL), 1, 0);

  ++service->n_intro_circuits_launched;
  launched = circuit_launch_by_extend_info(CIRCUIT_PURPOSE_S_ESTABLISH_INTRO,
                             launch_ei, flags);

  if (!launched) {
    log_info(LD_REND,
             "Can't launch circuit to establish introduction at %s%s%s.",
             safe_str_client(extend_info_describe(intro->extend_info)),
             direct_ei ? " via direct address " : "",
             direct_ei ? safe_str_client(extend_info_describe(direct_ei)) : ""
             );
    extend_info_free(direct_ei);
    return -1;
  }
   
  tor_assert(tor_memeq(intro->extend_info->identity_digest,
                       launched->build_state->chosen_exit->identity_digest,
                       DIGEST_LEN));

  launched->rend_data = rend_data_service_create(service->service_id,
                                                 service->pk_digest, NULL,
                                                 service->auth_type);
  launched->intro_key = crypto_pk_dup_key(intro->intro_key);
  if (launched->base_.state == CIRCUIT_STATE_OPEN)
    rend_service_intro_has_opened(launched);
  extend_info_free(direct_ei);
  return 0;
}
