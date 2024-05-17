choose_guard_selection(const or_options_t *options,
                       const networkstatus_t *live_ns,
                       const guard_selection_t *old_selection,
                       guard_selection_type_t *type_out)
{
  tor_assert(options);
  tor_assert(type_out);

  if (options->UseBridges) {
    *type_out = GS_TYPE_BRIDGE;
    return "bridges";
  }

  if (! live_ns) {
     
    *type_out = GS_TYPE_NORMAL;
    return "default";
  }

  const smartlist_t *nodes = nodelist_get_list();
  int n_guards = 0, n_passing_filter = 0;
  SMARTLIST_FOREACH_BEGIN(nodes, const node_t *, node) {
    if (node_is_possible_guard(node)) {
      ++n_guards;
      if (node_passes_guard_filter(options, node)) {
        ++n_passing_filter;
      }
    }
  } SMARTLIST_FOREACH_END(node);

   
  const int meaningful_threshold_high =
    (int)(n_guards * get_meaningful_restriction_threshold() * 1.05);
  const int meaningful_threshold_mid =
    (int)(n_guards * get_meaningful_restriction_threshold());
  const int meaningful_threshold_low =
    (int)(n_guards * get_meaningful_restriction_threshold() * .95);
  const int extreme_threshold =
    (int)(n_guards * get_extreme_restriction_threshold());

   

  static int have_warned_extreme_threshold = 0;
  if (n_guards &&
      n_passing_filter < extreme_threshold &&
      ! have_warned_extreme_threshold) {
    have_warned_extreme_threshold = 1;
    const double exclude_frac =
      (n_guards - n_passing_filter) / (double)n_guards;
    log_warn(LD_GUARD, "Your configuration excludes %d%% of all possible "
             "guards. That's likely to make you stand out from the "
             "rest of the world.", (int)(exclude_frac * 100));
  }

   
  if (old_selection == NULL) {
    if (n_passing_filter >= meaningful_threshold_mid) {
      *type_out = GS_TYPE_NORMAL;
      return "default";
    } else {
      *type_out = GS_TYPE_RESTRICTED;
      return "restricted";
    }
  }

   
  tor_assert(old_selection);

   
  if (n_passing_filter >= meaningful_threshold_high) {
    *type_out = GS_TYPE_NORMAL;
    return "default";
  } else if (n_passing_filter < meaningful_threshold_low) {
    *type_out = GS_TYPE_RESTRICTED;
    return "restricted";
  } else {
     
    *type_out = old_selection->type;
    return old_selection->name;
  }
}
