pp::Var ScriptableHandle::GetProperty(const pp::Var& name,
                                      pp::Var* exception) {
  PLUGIN_PRINTF(("ScriptableHandle::GetProperty (name=%s)\n",
                 name.DebugString().c_str()));
  if (plugin_ == NULL) {
    return pp::Var();
  }
  pp::Var property = Invoke(plugin_,
                            PROPERTY_GET,
                            NameAsString(name),
                            "GetProperty",
                            std::vector<pp::Var>(), exception);
  PLUGIN_PRINTF(("ScriptableHandle::GetProperty (property=%s)\n",
                 property.DebugString().c_str()));
  return property;
}
