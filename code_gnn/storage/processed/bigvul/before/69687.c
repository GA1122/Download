entry_guards_changed_for_guard_selection(guard_selection_t *gs)
{
  time_t when;

  tor_assert(gs != NULL);

  entry_guards_dirty = 1;

  if (get_options()->AvoidDiskWrites)
    when = time(NULL) + SLOW_GUARD_STATE_FLUSH_TIME;
  else
    when = time(NULL) + FAST_GUARD_STATE_FLUSH_TIME;

   
  or_state_mark_dirty(get_or_state(), when);
}
