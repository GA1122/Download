entry_guard_cancel(circuit_guard_state_t **guard_state_p)
{
  if (BUG(*guard_state_p == NULL))
    return;
  entry_guard_t *guard = entry_guard_handle_get((*guard_state_p)->guard);
  if (! guard)
    return;

   
  guard->is_pending = 0;
  circuit_guard_state_free(*guard_state_p);
  *guard_state_p = NULL;
}
