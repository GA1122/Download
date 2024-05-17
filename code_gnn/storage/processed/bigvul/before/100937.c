void ScriptableHandle::RemoveProperty(const pp::Var& name,
                                      pp::Var* exception) {
  PLUGIN_PRINTF(("ScriptableHandle::RemoveProperty (name=%s)\n",
                 name.DebugString().c_str()));
  Error(NameAsString(name), "RemoveProperty",
        "property removal is not supported", exception);
}
