void VaapiWrapper::DeinitializeVpp() {
  base::AutoLock auto_lock(*va_lock_);

  if (va_vpp_buffer_id_ != VA_INVALID_ID) {
    vaDestroyBuffer(va_display_, va_vpp_buffer_id_);
    va_vpp_buffer_id_ = VA_INVALID_ID;
  }
  if (va_vpp_context_id_ != VA_INVALID_ID) {
    vaDestroyContext(va_display_, va_vpp_context_id_);
    va_vpp_context_id_ = VA_INVALID_ID;
  }
  if (va_vpp_config_id_ != VA_INVALID_ID) {
    vaDestroyConfig(va_display_, va_vpp_config_id_);
    va_vpp_config_id_ = VA_INVALID_ID;
  }
}
