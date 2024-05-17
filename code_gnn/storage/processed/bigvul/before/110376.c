PluginInstance* PluginModule::GetSomeInstance() const {
  CHECK(!instances_.empty());
  return *instances_.begin();
}
