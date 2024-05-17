bool BrowserMainLoop::InitializeToolkit() {
  TRACE_EVENT0("startup", "BrowserMainLoop::InitializeToolkit");


#if defined(OS_WIN)
  INITCOMMONCONTROLSEX config;
  config.dwSize = sizeof(config);
  config.dwICC = ICC_WIN95_CLASSES;
  if (!InitCommonControlsEx(&config))
    PLOG(FATAL);
#endif

#if defined(USE_AURA)

#if defined(USE_X11)
  if (!parsed_command_line_.HasSwitch(switches::kHeadless) &&
      !gfx::GetXDisplay()) {
    LOG(ERROR) << "Unable to open X display.";
    return false;
  }
#endif

  env_ = aura::Env::CreateInstance(parameters_.env_mode);
#endif   

  if (parts_)
    parts_->ToolkitInitialized();

  return true;
}
