rend_service_verify_single_onion_poison(const rend_service_t* s,
                                        const or_options_t* options)
{
   
  if (BUG(!s)) {
    return -1;
  }

   
  if (BUG(rend_service_is_ephemeral(s))) {
    return -1;
  }

   
  if (BUG(!s->directory)) {
    return -1;
  }

   
  if (!rend_service_private_key_exists(s)) {
    return 0;
  }

   
  if (service_is_single_onion_poisoned(s) !=
      rend_service_non_anonymous_mode_enabled(options)) {
    return -1;
  }

   
  return 0;
}
