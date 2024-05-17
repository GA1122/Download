entry_guards_upgrade_waiting_circuits(guard_selection_t *gs,
                                      const smartlist_t *all_circuits_in,
                                      smartlist_t *newly_complete_out)
{
  tor_assert(gs);
  tor_assert(all_circuits_in);
  tor_assert(newly_complete_out);

  if (! entry_guards_all_primary_guards_are_down(gs)) {
     
    log_debug(LD_GUARD, "Considered upgrading guard-stalled circuits, "
              "but not all primary guards were definitely down.");
    return 0;
  }

  int n_waiting = 0;
  int n_complete = 0;
  int n_complete_blocking = 0;
  origin_circuit_t *best_waiting_circuit = NULL;
  smartlist_t *all_circuits = smartlist_new();
  SMARTLIST_FOREACH_BEGIN(all_circuits_in, origin_circuit_t *, circ) {
    circuit_guard_state_t *state = origin_circuit_get_guard_state(circ);
    if (state == NULL)
      continue;
    entry_guard_t *guard = entry_guard_handle_get(state->guard);
    if (!guard || guard->in_selection != gs)
      continue;

    smartlist_add(all_circuits, circ);
  } SMARTLIST_FOREACH_END(circ);

  SMARTLIST_FOREACH_BEGIN(all_circuits, origin_circuit_t *, circ) {
    circuit_guard_state_t *state = origin_circuit_get_guard_state(circ);
    if BUG((state == NULL))
      continue;

    if (state->state == GUARD_CIRC_STATE_WAITING_FOR_BETTER_GUARD) {
      ++n_waiting;
      if (! best_waiting_circuit ||
          circ_state_has_higher_priority(circ, NULL, best_waiting_circuit)) {
        best_waiting_circuit = circ;
      }
    }
  } SMARTLIST_FOREACH_END(circ);

  if (! best_waiting_circuit) {
    log_debug(LD_GUARD, "Considered upgrading guard-stalled circuits, "
              "but didn't find any.");
    goto no_change;
  }

   
  const entry_guard_restriction_t *rst_on_best_waiting =
    origin_circuit_get_guard_state(best_waiting_circuit)->restrictions;

   
  SMARTLIST_FOREACH_BEGIN(all_circuits, origin_circuit_t *, circ) {
     
    circuit_guard_state_t *state = origin_circuit_get_guard_state(circ);
    if BUG((state == NULL))
      continue;
    if (state->state != GUARD_CIRC_STATE_COMPLETE)
      continue;
    ++n_complete;
    if (circ_state_has_higher_priority(circ, rst_on_best_waiting,
                                       best_waiting_circuit))
      ++n_complete_blocking;
  } SMARTLIST_FOREACH_END(circ);

  if (n_complete_blocking) {
    log_debug(LD_GUARD, "Considered upgrading guard-stalled circuits: found "
              "%d complete and %d guard-stalled. At least one complete "
              "circuit had higher priority, so not upgrading.",
              n_complete, n_waiting);
    goto no_change;
  }

   
  int n_blockers_found = 0;
  const time_t state_set_at_cutoff =
    approx_time() - get_nonprimary_guard_connect_timeout();
  SMARTLIST_FOREACH_BEGIN(all_circuits, origin_circuit_t *, circ) {
    circuit_guard_state_t *state = origin_circuit_get_guard_state(circ);
    if (BUG(state == NULL))
      continue;
    if (state->state != GUARD_CIRC_STATE_USABLE_IF_NO_BETTER_GUARD)
      continue;
    if (state->state_set_at <= state_set_at_cutoff)
      continue;
    if (circ_state_has_higher_priority(circ, rst_on_best_waiting,
                                       best_waiting_circuit))
      ++n_blockers_found;
  } SMARTLIST_FOREACH_END(circ);

  if (n_blockers_found) {
    log_debug(LD_GUARD, "Considered upgrading guard-stalled circuits: found "
              "%d guard-stalled, but %d pending circuit(s) had higher "
              "guard priority, so not upgrading.",
              n_waiting, n_blockers_found);
    goto no_change;
  }

   
  int n_succeeded = 0;
  SMARTLIST_FOREACH_BEGIN(all_circuits, origin_circuit_t *, circ) {
    circuit_guard_state_t *state = origin_circuit_get_guard_state(circ);
    if (BUG(state == NULL))
      continue;
    if (circ != best_waiting_circuit && rst_on_best_waiting) {
       
      continue;
    }
    if (state->state != GUARD_CIRC_STATE_WAITING_FOR_BETTER_GUARD)
      continue;
    if (circ_state_has_higher_priority(best_waiting_circuit, NULL, circ))
      continue;

    state->state = GUARD_CIRC_STATE_COMPLETE;
    state->state_set_at = approx_time();
    smartlist_add(newly_complete_out, circ);
    ++n_succeeded;
  } SMARTLIST_FOREACH_END(circ);

  log_info(LD_GUARD, "Considered upgrading guard-stalled circuits: found "
           "%d guard-stalled, %d complete. %d of the guard-stalled "
           "circuit(s) had high enough priority to upgrade.",
           n_waiting, n_complete, n_succeeded);

  tor_assert_nonfatal(n_succeeded >= 1);
  smartlist_free(all_circuits);
  return 1;

 no_change:
  smartlist_free(all_circuits);
  return 0;
}