void ScriptableHandle::GetAllPropertyNames(std::vector<pp::Var>* properties,
                                           pp::Var* exception) {
  UNREFERENCED_PARAMETER(exception);
  PLUGIN_PRINTF(("ScriptableHandle::GetAllPropertyNames ()\n"));
  if (plugin_ == NULL) {
    return;
  }
  std::vector<uintptr_t>* ids = plugin_->GetPropertyIdentifiers();
  if (ids == NULL) {
    PLUGIN_PRINTF(("ScriptableHandle::GetAllPropertyNames "
                   "(ids=%p)\n", reinterpret_cast<void*>(ids)));
    return;
  }
  PLUGIN_PRINTF(("ScriptableHandle::GetAllPropertyNames "
                 "(ids->size()=%"NACL_PRIuS")\n", ids->size()));
  for (size_t i = 0; i < ids->size(); ++i) {
    nacl::string name =
        plugin_->browser_interface()->IdentifierToString(ids->at(i));
    properties->push_back(pp::Var(name));
  }
  PLUGIN_PRINTF(("ScriptableHandle::GetAllPropertyNames "
                 "(properties=%"NACL_PRIuS")\n", properties->size()));
}
