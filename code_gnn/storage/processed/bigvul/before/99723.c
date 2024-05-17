VaapiWrapper::VaapiWrapper()
    : va_display_(NULL),
      va_config_id_(VA_INVALID_ID),
      va_context_id_(VA_INVALID_ID),
      va_vpp_config_id_(VA_INVALID_ID),
      va_vpp_context_id_(VA_INVALID_ID),
      va_vpp_buffer_id_(VA_INVALID_ID) {
  va_lock_ = va_display_state_.Get().va_lock();
}
