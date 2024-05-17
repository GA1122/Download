bool Plugin::Init(int argc, char* argn[], char* argv[]) {
  PLUGIN_PRINTF(("Plugin::Init (instance=%p)\n", static_cast<void*>(this)));

#ifdef NACL_OSX
  pp::TextInput_Dev(this).SetTextInputType(PP_TEXTINPUT_TYPE_NONE);
#endif

  argn_ = new char*[argc];
  argv_ = new char*[argc];
  argc_ = 0;
  for (int i = 0; i < argc; ++i) {
    if (NULL != argn_ && NULL != argv_) {
      argn_[argc_] = strdup(argn[i]);
      argv_[argc_] = strdup(argv[i]);
      if (NULL == argn_[argc_] || NULL == argv_[argc_]) {
        free(argn_[argc_]);
        free(argv_[argc_]);
        continue;
      }
      ++argc_;
    }
  }

  wrapper_factory_ = new nacl::DescWrapperFactory();
  if (NULL == wrapper_factory_) {
    return false;
  }
  PLUGIN_PRINTF(("Plugin::Init (wrapper_factory=%p)\n",
                 static_cast<void*>(wrapper_factory_)));

  AddPropertyGet("exitStatus", &Plugin::GetExitStatus);
  AddPropertyGet("lastError", &Plugin::GetLastError);
  AddPropertyGet("readyState", &Plugin::GetReadyStateProperty);

  PLUGIN_PRINTF(("Plugin::Init (return 1)\n"));
  return true;
}
