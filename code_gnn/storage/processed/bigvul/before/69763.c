compute_frac_paths_available(const networkstatus_t *consensus,
                             const or_options_t *options, time_t now,
                             int *num_present_out, int *num_usable_out,
                             char **status_out)
{
  smartlist_t *guards = smartlist_new();
  smartlist_t *mid    = smartlist_new();
  smartlist_t *exits  = smartlist_new();
  double f_guard, f_mid, f_exit;
  double f_path = 0.0;
   
  int np = 0;
   
  int nu = 0;
  const int authdir = authdir_mode_v3(options);

  count_usable_descriptors(num_present_out, num_usable_out,
                           mid, consensus, options, now, NULL,
                           USABLE_DESCRIPTOR_ALL);
  if (options->EntryNodes) {
    count_usable_descriptors(&np, &nu, guards, consensus, options, now,
                             options->EntryNodes, USABLE_DESCRIPTOR_ALL);
  } else {
    SMARTLIST_FOREACH(mid, const node_t *, node, {
      if (authdir) {
        if (node->rs && node->rs->is_possible_guard)
          smartlist_add(guards, (node_t*)node);
      } else {
        if (node->is_possible_guard)
          smartlist_add(guards, (node_t*)node);
      }
    });
  }

   
  count_usable_descriptors(&np, &nu, exits, consensus, options, now,
                           NULL, USABLE_DESCRIPTOR_EXIT_ONLY);
  log_debug(LD_NET,
            "%s: %d present, %d usable",
            "exits",
            np,
            nu);

   
   
  consensus_path_type_t old_have_consensus_path = have_consensus_path;
  have_consensus_path = ((nu > 0) ?
                         CONSENSUS_PATH_EXIT :
                         CONSENSUS_PATH_INTERNAL);

  if (have_consensus_path == CONSENSUS_PATH_INTERNAL
      && old_have_consensus_path != have_consensus_path) {
    log_notice(LD_NET,
               "The current consensus has no exit nodes. "
               "Tor can only build internal paths, "
               "such as paths to hidden services.");

     
  }

  f_guard = frac_nodes_with_descriptors(guards, WEIGHT_FOR_GUARD);
  f_mid   = frac_nodes_with_descriptors(mid,    WEIGHT_FOR_MID);
  f_exit  = frac_nodes_with_descriptors(exits,  WEIGHT_FOR_EXIT);

  log_debug(LD_NET,
            "f_guard: %.2f, f_mid: %.2f, f_exit: %.2f",
             f_guard,
             f_mid,
             f_exit);

  smartlist_free(guards);
  smartlist_free(mid);
  smartlist_free(exits);

  if (options->ExitNodes) {
    double f_myexit, f_myexit_unflagged;
    smartlist_t *myexits= smartlist_new();
    smartlist_t *myexits_unflagged = smartlist_new();

     
    count_usable_descriptors(&np, &nu, myexits, consensus, options, now,
                             options->ExitNodes, USABLE_DESCRIPTOR_EXIT_ONLY);
    log_debug(LD_NET,
              "%s: %d present, %d usable",
              "myexits",
              np,
              nu);

     
    count_usable_descriptors(&np, &nu, myexits_unflagged,
                             consensus, options, now,
                             options->ExitNodes, USABLE_DESCRIPTOR_ALL);
    log_debug(LD_NET,
              "%s: %d present, %d usable",
              "myexits_unflagged (initial)",
              np,
              nu);

    SMARTLIST_FOREACH_BEGIN(myexits_unflagged, const node_t *, node) {
      if (node_has_descriptor(node) && node_exit_policy_rejects_all(node)) {
        SMARTLIST_DEL_CURRENT(myexits_unflagged, node);
         
        np--;
         
        nu--;
      }
    } SMARTLIST_FOREACH_END(node);

    log_debug(LD_NET,
              "%s: %d present, %d usable",
              "myexits_unflagged (final)",
              np,
              nu);

    f_myexit= frac_nodes_with_descriptors(myexits,WEIGHT_FOR_EXIT);
    f_myexit_unflagged=
              frac_nodes_with_descriptors(myexits_unflagged,WEIGHT_FOR_EXIT);

    log_debug(LD_NET,
              "f_exit: %.2f, f_myexit: %.2f, f_myexit_unflagged: %.2f",
              f_exit,
              f_myexit,
              f_myexit_unflagged);

     
    if (smartlist_len(myexits) == 0 &&
        smartlist_len(myexits_unflagged)) {
      f_myexit = f_myexit_unflagged;
    }

    smartlist_free(myexits);
    smartlist_free(myexits_unflagged);

     
    if (f_myexit < f_exit)
      f_exit = f_myexit;
  }

   
  if (router_have_consensus_path() != CONSENSUS_PATH_EXIT) {
    f_exit = 1.0;
  }

  f_path = f_guard * f_mid * f_exit;

  if (status_out)
    tor_asprintf(status_out,
                 "%d%% of guards bw, "
                 "%d%% of midpoint bw, and "
                 "%d%% of exit bw%s = "
                 "%d%% of path bw",
                 (int)(f_guard*100),
                 (int)(f_mid*100),
                 (int)(f_exit*100),
                 (router_have_consensus_path() == CONSENSUS_PATH_EXIT ?
                  "" :
                  " (no exits in consensus)"),
                 (int)(f_path*100));

  return f_path;
}