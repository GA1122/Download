bool VaapiWrapper::VaInitialize(const base::Closure& report_error_to_uma_cb) {
  static bool vaapi_functions_initialized = PostSandboxInitialization();
  if (!vaapi_functions_initialized) {
    bool running_on_chromeos = false;
#if defined(OS_CHROMEOS)
    running_on_chromeos = base::SysInfo::IsRunningOnChromeOS();
#endif
    static const char kErrorMsg[] = "Failed to initialize VAAPI libs";
    if (running_on_chromeos)
      LOG(ERROR) << kErrorMsg;
    else
      DVLOG(1) << kErrorMsg;
    return false;
  }

  report_error_to_uma_cb_ = report_error_to_uma_cb;

  base::AutoLock auto_lock(*va_lock_);

  VADisplayState* va_display_state = &va_display_state_.Get();
  if (!va_display_state) {
    LOG(ERROR) << "Failed to allocate VA display state";
    return false;
  }

  VAStatus va_res = VA_STATUS_SUCCESS;
  if (!va_display_state->Initialize(&va_res)) {
    VA_LOG_ON_ERROR(va_res, "vaInitialize failed");
    return false;
  }

  va_display_ = va_display_state->va_display();
  return true;
}
