should_apply_guardfraction(const networkstatus_t *ns)
{
   
  const or_options_t *options = get_options();

   
  if (options->UseGuardFraction == -1) {
    return networkstatus_get_param(ns, "UseGuardFraction",
                                   0,  
                                   0, 1);
  }

  return options->UseGuardFraction;
}
