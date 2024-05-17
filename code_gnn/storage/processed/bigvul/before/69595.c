rend_consider_services_intro_points(void)
{
  int i;
  time_t now;
  const or_options_t *options = get_options();
   
  const int allow_direct = rend_service_allow_non_anonymous_connection(
                                                                get_options());
   
  smartlist_t *exclude_nodes;
   
  smartlist_t *retry_nodes;

  if (!have_completed_a_circuit())
    return;

  exclude_nodes = smartlist_new();
  retry_nodes = smartlist_new();
  now = time(NULL);

  SMARTLIST_FOREACH_BEGIN(rend_service_list, rend_service_t *, service) {
    int r;
     
    unsigned int n_intro_points_to_open;
     
    unsigned int intro_nodes_len;
     
    smartlist_clear(exclude_nodes);
    smartlist_clear(retry_nodes);

     
    remove_invalid_intro_points(service, exclude_nodes, retry_nodes, now);

     
    if (now > service->intro_period_started + INTRO_CIRC_RETRY_PERIOD) {
       
      service->intro_period_started = now;
      service->n_intro_circuits_launched = 0;
    } else if (service->n_intro_circuits_launched >=
               rend_max_intro_circs_per_period(
                                      service->n_intro_points_wanted)) {
       
      continue;
    }

     
    SMARTLIST_FOREACH_BEGIN(retry_nodes, rend_intro_point_t *, intro) {
      r = rend_service_launch_establish_intro(service, intro);
      if (r < 0) {
        log_warn(LD_REND, "Error launching circuit to node %s for service %s.",
                 safe_str_client(extend_info_describe(intro->extend_info)),
                 safe_str_client(service->service_id));
         
        smartlist_remove(service->intro_nodes, intro);
        rend_intro_point_free(intro);
        continue;
      }
      intro->circuit_retries++;
    } SMARTLIST_FOREACH_END(intro);

     
    intro_nodes_len = (unsigned int) smartlist_len(service->intro_nodes);

     
    if (intro_nodes_len >= service->n_intro_points_wanted) {
      continue;
    }

     
    n_intro_points_to_open = service->n_intro_points_wanted - intro_nodes_len;
    if (intro_nodes_len == 0) {
       
      n_intro_points_to_open += NUM_INTRO_POINTS_EXTRA;
    }

    for (i = 0; i < (int) n_intro_points_to_open; i++) {
      const node_t *node;
      rend_intro_point_t *intro;
      router_crn_flags_t flags = CRN_NEED_UPTIME|CRN_NEED_DESC;
      if (get_options()->AllowInvalid_ & ALLOW_INVALID_INTRODUCTION)
        flags |= CRN_ALLOW_INVALID;
      router_crn_flags_t direct_flags = flags;
      direct_flags |= CRN_PREF_ADDR;
      direct_flags |= CRN_DIRECT_CONN;

      node = router_choose_random_node(exclude_nodes,
                                       options->ExcludeNodes,
                                       allow_direct ? direct_flags : flags);
       
      if (allow_direct && !node) {
        log_info(LD_REND,
                 "Unable to find an intro point that we can connect to "
                 "directly for %s, falling back to a 3-hop path.",
                 safe_str_client(service->service_id));
        node = router_choose_random_node(exclude_nodes,
                                         options->ExcludeNodes, flags);
      }

      if (!node) {
        log_warn(LD_REND,
                 "We only have %d introduction points established for %s; "
                 "wanted %u.",
                 smartlist_len(service->intro_nodes),
                 safe_str_client(service->service_id),
                 n_intro_points_to_open);
        break;
      }
       
      smartlist_add(exclude_nodes, (void*)node);
      intro = tor_malloc_zero(sizeof(rend_intro_point_t));
       
      intro->extend_info = extend_info_from_node(node, 0);
      if (BUG(intro->extend_info == NULL)) {
        break;
      }
      intro->intro_key = crypto_pk_new();
      const int fail = crypto_pk_generate_key(intro->intro_key);
      tor_assert(!fail);
      intro->time_published = -1;
      intro->time_to_expire = -1;
      intro->max_introductions =
        crypto_rand_int_range(INTRO_POINT_MIN_LIFETIME_INTRODUCTIONS,
                              INTRO_POINT_MAX_LIFETIME_INTRODUCTIONS);
      smartlist_add(service->intro_nodes, intro);
      log_info(LD_REND, "Picked router %s as an intro point for %s.",
               safe_str_client(node_describe(node)),
               safe_str_client(service->service_id));
       
      r = rend_service_launch_establish_intro(service, intro);
      if (r < 0) {
        log_warn(LD_REND, "Error launching circuit to node %s for service %s.",
                 safe_str_client(extend_info_describe(intro->extend_info)),
                 safe_str_client(service->service_id));
         
      }
    }
  } SMARTLIST_FOREACH_END(service);
  smartlist_free(exclude_nodes);
  smartlist_free(retry_nodes);
}
