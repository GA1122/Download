void Plugin::AddPropertyGet(const nacl::string& prop_name,
                            Plugin::PropertyGetter getter) {
  PLUGIN_PRINTF(("Plugin::AddPropertyGet (prop_name='%s')\n",
                 prop_name.c_str()));
  property_getters_[nacl::string(prop_name)] = getter;
}
