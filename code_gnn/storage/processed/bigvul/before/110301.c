bool Plugin::HasProperty(const nacl::string& prop_name) {
  PLUGIN_PRINTF(("Plugin::HasProperty (prop_name=%s)\n",
                 prop_name.c_str()));
  return property_getters_[prop_name] != NULL;
}
