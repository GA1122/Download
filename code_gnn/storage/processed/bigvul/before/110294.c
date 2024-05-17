bool Plugin::GetProperty(const nacl::string& prop_name,
                         NaClSrpcArg* prop_value) {
  PLUGIN_PRINTF(("Plugin::GetProperty (prop_name=%s)\n", prop_name.c_str()));

  PropertyGetter getter = property_getters_[prop_name];
  if (NULL == getter) {
    return false;
  }
  (this->*getter)(prop_value);
  return true;
}
