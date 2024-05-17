void ScriptableHandle::SetProperty(const pp::Var& name,
                                   const pp::Var& value,
                                   pp::Var* exception) {
  PLUGIN_PRINTF(("ScriptableHandle::SetProperty (name=%s, value=%s)\n",
                 name.DebugString().c_str(), value.DebugString().c_str()));
  if (plugin_ == NULL) {
    return;
  }
  std::vector<pp::Var> args;
  args.push_back(pp::Var(pp::Var::DontManage(), value.pp_var()));
  Invoke(plugin_,
         PROPERTY_SET,
         NameAsString(name),
         "SetProperty",
         args,
         exception);
  std::string exception_string("NULL");
  if (!exception->is_undefined()) {
    exception_string = exception->DebugString();
  }
  PLUGIN_PRINTF(("ScriptableHandle::SetProperty (exception=%s)\n",
                 exception_string.c_str()));
}
