void VaapiWrapper::VADisplayState::Deinitialize(VAStatus* status) {
  va_lock_.AssertAcquired();
  if (--refcount_ > 0)
    return;

  if (va_initialized_ && va_display_) {
    *status = vaTerminate(va_display_);
  }
  va_initialized_ = false;
  va_display_ = nullptr;
}
