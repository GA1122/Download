entry_guards_note_guard_success(guard_selection_t *gs,
                                entry_guard_t *guard,
                                unsigned old_state)
{
  tor_assert(gs);

   
  const time_t last_time_on_internet = gs->last_time_on_internet;
  gs->last_time_on_internet = approx_time();

  guard->is_reachable = GUARD_REACHABLE_YES;
  guard->failing_since = 0;
  guard->is_pending = 0;
  if (guard->is_filtered_guard)
    guard->is_usable_filtered_guard = 1;

  if (guard->confirmed_idx < 0) {
    make_guard_confirmed(gs, guard);
    if (!gs->primary_guards_up_to_date)
      entry_guards_update_primary(gs);
  }

  unsigned new_state;
  switch (old_state) {
    case GUARD_CIRC_STATE_COMPLETE:
    case GUARD_CIRC_STATE_USABLE_ON_COMPLETION:
      new_state = GUARD_CIRC_STATE_COMPLETE;
      break;
    default:
      tor_assert_nonfatal_unreached();
       
    case GUARD_CIRC_STATE_USABLE_IF_NO_BETTER_GUARD:
      if (guard->is_primary) {
         
         
        new_state = GUARD_CIRC_STATE_COMPLETE;
      } else {
        new_state = GUARD_CIRC_STATE_WAITING_FOR_BETTER_GUARD;
      }
      break;
  }

  if (! guard->is_primary) {
    if (last_time_on_internet + get_internet_likely_down_interval()
        < approx_time()) {
      mark_primary_guards_maybe_reachable(gs);
    }
  }

  log_info(LD_GUARD, "Recorded success for %s%sguard %s",
           guard->is_primary?"primary ":"",
           guard->confirmed_idx>=0?"confirmed ":"",
           entry_guard_describe(guard));

  return new_state;
}
