rend_service_non_anonymous_mode_consistent(const or_options_t *options)
{
   
  return (!! options->HiddenServiceSingleHopMode ==
          !! options->HiddenServiceNonAnonymousMode);
}