bool ScriptableHandle::HasMethod(const pp::Var& name, pp::Var* exception) {
  UNREFERENCED_PARAMETER(exception);
  PLUGIN_PRINTF(("ScriptableHandle::HasMethod (this=%p, name='%s')\n",
                 static_cast<void*>(this), name.DebugString().c_str()));
  if (plugin_ == NULL) {
    return false;
  }
  if (!name.is_string())
    return false;
  bool has_method = HasCallType(plugin_,
                                METHOD_CALL,
                                name.AsString(),
                                "HasMethod");
  PLUGIN_PRINTF(("ScriptableHandle::HasMethod (has_method=%d)\n",
                 has_method));
  return has_method;
}
