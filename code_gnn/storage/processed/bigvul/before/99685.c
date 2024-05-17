void VaapiWrapper::Deinitialize() {
  base::AutoLock auto_lock(*va_lock_);

  if (va_config_id_ != VA_INVALID_ID) {
    VAStatus va_res = vaDestroyConfig(va_display_, va_config_id_);
    VA_LOG_ON_ERROR(va_res, "vaDestroyConfig failed");
  }

  VADisplayState* va_display_state = &va_display_state_.Get();
  if (va_display_state) {
    VAStatus va_res = VA_STATUS_SUCCESS;
    va_display_state->Deinitialize(&va_res);
    VA_LOG_ON_ERROR(va_res, "vaTerminate failed");
  }

  va_config_id_ = VA_INVALID_ID;
  va_display_ = NULL;
}
