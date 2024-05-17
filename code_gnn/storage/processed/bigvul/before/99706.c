bool VaapiWrapper::InitializeVpp_Locked() {
  va_lock_->AssertAcquired();

  VA_SUCCESS_OR_RETURN(
      vaCreateConfig(va_display_, VAProfileNone, VAEntrypointVideoProc, NULL, 0,
                     &va_vpp_config_id_),
      "Couldn't create config", false);

  VA_SUCCESS_OR_RETURN(vaCreateContext(va_display_, va_vpp_config_id_, 1, 1, 0,
                                       NULL, 0, &va_vpp_context_id_),
                       "Couldn't create context", false);

  VA_SUCCESS_OR_RETURN(vaCreateBuffer(va_display_, va_vpp_context_id_,
                                      VAProcPipelineParameterBufferType,
                                      sizeof(VAProcPipelineParameterBuffer), 1,
                                      NULL, &va_vpp_buffer_id_),
                       "Couldn't create buffer", false);

  return true;
}
