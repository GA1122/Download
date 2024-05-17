remove_invalid_intro_points(rend_service_t *service,
                            smartlist_t *exclude_nodes,
                            smartlist_t *retry_nodes, time_t now)
{
  tor_assert(service);

   
  SMARTLIST_FOREACH_BEGIN(service->expiring_nodes, rend_intro_point_t *,
                          intro) {
    origin_circuit_t *intro_circ =
      find_intro_circuit(intro, service->pk_digest);
    if (intro_circ) {
      continue;
    }
     
    SMARTLIST_DEL_CURRENT(service->expiring_nodes, intro);
    rend_intro_point_free(intro);
  } SMARTLIST_FOREACH_END(intro);

  SMARTLIST_FOREACH_BEGIN(service->intro_nodes, rend_intro_point_t *,
                          intro) {
     
    const node_t *node =
      node_get_by_id(intro->extend_info->identity_digest);
     
    origin_circuit_t *intro_circ =
      find_intro_circuit(intro, service->pk_digest);

     
    if (node && exclude_nodes) {
      smartlist_add(exclude_nodes, (void*) node);
    }

     
    if (intro_circ == NULL) {
      log_info(LD_REND, "Attempting to retry on %s as intro point for %s"
               " (circuit disappeared).",
               safe_str_client(extend_info_describe(intro->extend_info)),
               safe_str_client(service->service_id));
       
      intro->circuit_established = 0;

       
      if (node == NULL ||
          intro->circuit_retries >= MAX_INTRO_POINT_CIRCUIT_RETRIES) {
        rend_intro_point_free(intro);
        SMARTLIST_DEL_CURRENT(service->intro_nodes, intro);
         
        continue;
      }

       
      if (retry_nodes) {
        smartlist_add(retry_nodes, intro);
      }
    }
     

     
    if (intro_point_should_expire_now(intro, now)) {
      log_info(LD_REND, "Expiring %s as intro point for %s.",
               safe_str_client(extend_info_describe(intro->extend_info)),
               safe_str_client(service->service_id));
      smartlist_add(service->expiring_nodes, intro);
      SMARTLIST_DEL_CURRENT(service->intro_nodes, intro);
       
      intro->circuit_established = 0;
    }
  } SMARTLIST_FOREACH_END(intro);
}
