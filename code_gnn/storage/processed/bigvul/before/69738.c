live_consensus_is_missing(const guard_selection_t *gs)
{
  tor_assert(gs);
  if (gs->type == GS_TYPE_BRIDGE) {
     
    return 0;
  }
  return networkstatus_get_live_consensus(approx_time()) == NULL;
}
