NPError WebPluginDelegatePepper::Device3DGetNumConfigs(int32* num_configs) {
  if (!num_configs)
    return NPERR_GENERIC_ERROR;

  *num_configs = 1;
  return NPERR_NO_ERROR;
}
