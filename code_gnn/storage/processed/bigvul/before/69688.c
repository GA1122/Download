entry_guards_expand_sample(guard_selection_t *gs)
{
  tor_assert(gs);
  const or_options_t *options = get_options();

  if (live_consensus_is_missing(gs)) {
    log_info(LD_GUARD, "Not expanding the sample guard set; we have "
             "no live consensus.");
    return NULL;
  }

  int n_sampled = smartlist_len(gs->sampled_entry_guards);
  entry_guard_t *added_guard = NULL;
  int n_usable_filtered_guards = num_reachable_filtered_guards(gs, NULL);
  int n_guards = 0;
  smartlist_t *eligible_guards = get_eligible_guards(options, gs, &n_guards);

  const int max_sample = get_max_sample_size(gs, n_guards);
  const int min_filtered_sample = get_min_filtered_sample_size();

  log_info(LD_GUARD, "Expanding the sample guard set. We have %d guards "
           "in the sample, and %d eligible guards to extend it with.",
           n_sampled, smartlist_len(eligible_guards));

  while (n_usable_filtered_guards < min_filtered_sample) {
     
    if (n_sampled >= max_sample) {
      log_info(LD_GUARD, "Not expanding the guard sample any further; "
               "just hit the maximum sample threshold of %d",
               max_sample);
      goto done;
    }

     
    if (smartlist_len(eligible_guards) == 0) {
       
      log_info(LD_GUARD, "Not expanding the guard sample any further; "
               "just ran out of eligible guards");
      goto done;
       
    }

     
    added_guard = select_and_add_guard_item_for_sample(gs, eligible_guards);
    if (!added_guard)
      goto done;  

    ++n_sampled;

    if (added_guard->is_usable_filtered_guard)
      ++n_usable_filtered_guards;
  }

 done:
  smartlist_free(eligible_guards);
  return added_guard;
}
