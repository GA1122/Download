bool Plugin::LoadNaClModule(nacl::DescWrapper* wrapper,
                            ErrorInfo* error_info,
                            pp::CompletionCallback init_done_cb,
                            pp::CompletionCallback crash_cb) {
  ShutDownSubprocesses();
  if (!LoadNaClModuleCommon(wrapper, &main_subprocess_, manifest_.get(),
                            true, error_info, init_done_cb, crash_cb)) {
    return false;
  }
  PLUGIN_PRINTF(("Plugin::LoadNaClModule (%s)\n",
                 main_subprocess_.detailed_description().c_str()));
  return true;
 }
