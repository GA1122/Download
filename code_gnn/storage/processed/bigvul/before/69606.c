rend_service_check_private_dir(const or_options_t *options,
                               const rend_service_t *s,
                               int create)
{
   
  if (BUG(!s)) {
    return -1;
  }

   
  if (rend_service_check_private_dir_impl(options, s, create) < 0) {
    return -1;
  }

   
  if (rend_service_verify_single_onion_poison(s, options) < 0) {
     
    log_warn(LD_GENERAL, "We are configured with "
             "HiddenServiceNonAnonymousMode %d, but the hidden "
             "service key in directory %s was created in %s mode. "
             "This is not allowed.",
             rend_service_non_anonymous_mode_enabled(options) ? 1 : 0,
             rend_service_escaped_dir(s),
             rend_service_non_anonymous_mode_enabled(options) ?
             "an anonymous" : "a non-anonymous"
             );
    return -1;
  }

   
  if (create && rend_service_non_anonymous_mode_enabled(options)) {
    static int logged_warning = 0;

    if (rend_service_poison_new_single_onion_dir(s, options) < 0) {
      log_warn(LD_GENERAL,"Failed to mark new hidden services as non-anonymous"
               ".");
      return -1;
    }

    if (!logged_warning) {
       
      log_notice(LD_REND, "The configured onion service directories have been "
                 "used in single onion mode. They can not be used for "
                 "anonymous hidden services.");
      logged_warning = 1;
    }
  }

  return 0;
}
