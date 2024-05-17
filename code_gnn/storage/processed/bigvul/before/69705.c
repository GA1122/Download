get_eligible_guards(const or_options_t *options,
                    guard_selection_t *gs,
                    int *n_guards_out)
{
   
  smartlist_t *eligible_guards = smartlist_new();
  int n_guards = 0;  

  if (gs->type == GS_TYPE_BRIDGE) {
    const smartlist_t *bridges = bridge_list_get();
    SMARTLIST_FOREACH_BEGIN(bridges, bridge_info_t *, bridge) {
      ++n_guards;
      if (NULL != get_sampled_guard_for_bridge(gs, bridge)) {
        continue;
      }
      smartlist_add(eligible_guards, bridge);
    } SMARTLIST_FOREACH_END(bridge);
  } else {
    const smartlist_t *nodes = nodelist_get_list();
    const int n_sampled = smartlist_len(gs->sampled_entry_guards);

     
    digestset_t *sampled_guard_ids = digestset_new(n_sampled);
    SMARTLIST_FOREACH_BEGIN(gs->sampled_entry_guards, const entry_guard_t *,
                            guard) {
      digestset_add(sampled_guard_ids, guard->identity);
    } SMARTLIST_FOREACH_END(guard);

    SMARTLIST_FOREACH_BEGIN(nodes, const node_t *, node) {
      if (! node_is_possible_guard(node))
        continue;
      if (gs->type == GS_TYPE_RESTRICTED) {
         
        if (! node_passes_guard_filter(options, node))
          continue;
      }
      ++n_guards;
      if (digestset_contains(sampled_guard_ids, node->identity))
        continue;
      smartlist_add(eligible_guards, (node_t*)node);
    } SMARTLIST_FOREACH_END(node);

     
    digestset_free(sampled_guard_ids);
  }

  *n_guards_out = n_guards;
  return eligible_guards;
}
