bool VaapiWrapper::VADisplayState::Initialize(VAStatus* status) {
  va_lock_.AssertAcquired();
  if (refcount_++ == 0) {
#if defined(USE_X11)
    va_display_ = vaGetDisplay(gfx::GetXDisplay());
#elif defined(USE_OZONE)
    va_display_ = vaGetDisplayDRM(drm_fd_.get());
#endif   

    if (!vaDisplayIsValid(va_display_)) {
      LOG(ERROR) << "Could not get a valid VA display";
      return false;
    }

    *status = vaInitialize(va_display_, &major_version_, &minor_version_);
    if (*status != VA_STATUS_SUCCESS)
      return false;

    va_initialized_ = true;
    DVLOG(1) << "VAAPI version: " << major_version_ << "." << minor_version_;
  }

  if (VAAPIVersionLessThan(0, 34)) {
    LOG(ERROR) << "VAAPI version < 0.34 is not supported.";
    return false;
  }
  return true;
}
