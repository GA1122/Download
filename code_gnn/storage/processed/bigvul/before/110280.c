void Plugin::AddToConsole(const nacl::string& text) {
  pp::Module* module = pp::Module::Get();
  const PPB_Var* var_interface =
      static_cast<const PPB_Var*>(
          module->GetBrowserInterface(PPB_VAR_INTERFACE));
  nacl::string prefix_string("NativeClient");
  PP_Var prefix =
      var_interface->VarFromUtf8(prefix_string.c_str(),
                                 static_cast<uint32_t>(prefix_string.size()));
  PP_Var str = var_interface->VarFromUtf8(text.c_str(),
                                          static_cast<uint32_t>(text.size()));
  const PPB_Console_Dev* console_interface =
      static_cast<const PPB_Console_Dev*>(
          module->GetBrowserInterface(PPB_CONSOLE_DEV_INTERFACE));
  console_interface->LogWithSource(pp_instance(), PP_LOGLEVEL_LOG, prefix, str);
  var_interface->Release(prefix);
  var_interface->Release(str);
}
