bool VaapiWrapper::SubmitVAEncMiscParamBuffer(
    VAEncMiscParameterType misc_param_type,
    size_t size,
    void* buffer) {
  base::AutoLock auto_lock(*va_lock_);

  VABufferID buffer_id;
  VAStatus va_res = vaCreateBuffer(va_display_,
                                   va_context_id_,
                                   VAEncMiscParameterBufferType,
                                   sizeof(VAEncMiscParameterBuffer) + size,
                                   1,
                                   NULL,
                                   &buffer_id);
  VA_SUCCESS_OR_RETURN(va_res, "Failed to create a VA buffer", false);

  void* data_ptr = NULL;
  va_res = vaMapBuffer(va_display_, buffer_id, &data_ptr);
  VA_LOG_ON_ERROR(va_res, "vaMapBuffer failed");
  if (va_res != VA_STATUS_SUCCESS) {
    vaDestroyBuffer(va_display_, buffer_id);
    return false;
  }

  DCHECK(data_ptr);

  VAEncMiscParameterBuffer* misc_param =
      reinterpret_cast<VAEncMiscParameterBuffer*>(data_ptr);
  misc_param->type = misc_param_type;
  memcpy(misc_param->data, buffer, size);
  va_res = vaUnmapBuffer(va_display_, buffer_id);
  VA_LOG_ON_ERROR(va_res, "vaUnmapBuffer failed");

  pending_va_bufs_.push_back(buffer_id);
  return true;
}
