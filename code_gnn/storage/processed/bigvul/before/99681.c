bool VaapiWrapper::CreateCodedBuffer(size_t size, VABufferID* buffer_id) {
  base::AutoLock auto_lock(*va_lock_);
  VAStatus va_res = vaCreateBuffer(va_display_,
                                   va_context_id_,
                                   VAEncCodedBufferType,
                                   size,
                                   1,
                                   NULL,
                                   buffer_id);
  VA_SUCCESS_OR_RETURN(va_res, "Failed to create a coded buffer", false);

  const auto is_new_entry = coded_buffers_.insert(*buffer_id).second;
  DCHECK(is_new_entry);
  return true;
}
