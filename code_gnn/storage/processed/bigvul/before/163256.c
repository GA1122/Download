void BrowserMainLoop::EarlyInitialization() {
  TRACE_EVENT0("startup", "BrowserMainLoop::EarlyInitialization");

#if defined(OS_POSIX) && !defined(OS_MACOSX) && !defined(OS_ANDROID) && \
    !defined(OS_FUCHSIA)
  SetupSandbox(parsed_command_line_);
#endif

#if defined(USE_X11)
  if (UsingInProcessGpu()) {
    if (!gfx::InitializeThreadedX11()) {
      LOG(ERROR) << "Failed to put Xlib into threaded mode.";
    }
  }
#endif

#if defined(USE_GLIB)
#if !GLIB_CHECK_VERSION(2, 35, 0)
  g_type_init();
#endif   

  SetUpGLibLogHandler();
#endif   

  if (parts_)
    parts_->PreEarlyInitialization();

#if defined(OS_MACOSX) || defined(OS_LINUX) || defined(OS_CHROMEOS)

  base::SetFdLimit(8192);
#endif   

#if defined(OS_WIN)
  net::EnsureWinsockInit();
#endif

#if defined(USE_NSS_CERTS)
  crypto::EnsureNSPRInit();
#endif

#if defined(OS_FUCHSIA)
  InitDefaultJob();
#endif

  if (parsed_command_line_.HasSwitch(switches::kRendererProcessLimit)) {
    std::string limit_string = parsed_command_line_.GetSwitchValueASCII(
        switches::kRendererProcessLimit);
    size_t process_limit;
    if (base::StringToSizeT(limit_string, &process_limit)) {
      RenderProcessHost::SetMaxRendererProcessCount(process_limit);
    }
  }

#if defined(OS_ANDROID)
  ui::ViewAndroid::SetIsUseZoomForDSFEnabled(IsUseZoomForDSFEnabled());
#endif

  if (parts_)
    parts_->PostEarlyInitialization();
}
