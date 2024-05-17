pp::Var ScriptableHandle::Call(const pp::Var& name,
                               const std::vector<pp::Var>& args,
                               pp::Var* exception) {
  PLUGIN_PRINTF(("ScriptableHandle::Call (name=%s, %"NACL_PRIuS
                 " args)\n", name.DebugString().c_str(), args.size()));
  if (plugin_ == NULL) {
    return pp::Var();
  }
  if (name.is_undefined())   
    return pp::Var();
  assert(name.is_string());
  pp::Var return_var = Invoke(plugin_,
                              METHOD_CALL,
                              name.AsString(),
                              "Call",
                              args,
                              exception);
  PLUGIN_PRINTF(("ScriptableHandle::Call (return=%s)\n",
                 return_var.DebugString().c_str()));
  return return_var;
}
