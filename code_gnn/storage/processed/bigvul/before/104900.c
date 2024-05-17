int32_t PPB_URLLoader_Impl::ValidateCallback(PP_CompletionCallback callback) {
  if (!callback.func)
    return PP_ERROR_BADARGUMENT;

  if (pending_callback_.get() && !pending_callback_->completed())
    return PP_ERROR_INPROGRESS;

  return PP_OK;
}
