entry_guard_has_higher_priority(entry_guard_t *a, entry_guard_t *b)
{
  tor_assert(a && b);
  if (a == b)
    return 0;

   
  if (a->confirmed_idx < 0) {
    if (b->confirmed_idx >= 0)
      return 0;
  } else {
    if (b->confirmed_idx < 0)
      return 1;

     
    return (a->confirmed_idx < b->confirmed_idx);
  }

   
  if (a->is_pending) {
    if (! b->is_pending)
      return 1;

     
    return a->last_tried_to_connect < b->last_tried_to_connect;
  } else {
    if (b->is_pending)
      return 0;

     
    return 0;
  }
}
