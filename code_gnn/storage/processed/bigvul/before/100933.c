bool ScriptableHandle::HasProperty(const pp::Var& name, pp::Var* exception) {
  UNREFERENCED_PARAMETER(exception);
  PLUGIN_PRINTF(("ScriptableHandle::HasProperty (this=%p, name=%s)\n",
                 static_cast<void*>(this), name.DebugString().c_str()));
  if (plugin_ == NULL) {
    return false;
  }
  if (!name.is_string() && !name.is_int())
    return false;
  bool has_property = HasCallType(plugin_,
                                  PROPERTY_GET,
                                  name.AsString(),
                                  "HasProperty");
  PLUGIN_PRINTF(("ScriptableHandle::HasProperty (has_property=%d)\n",
                 has_property));
  return has_property;
}
