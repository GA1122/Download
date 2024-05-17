bool GLES2DecoderImpl::CheckCurrentValuebufferForSubscription(
    GLenum subscription,
    const char* function_name) {
  if (!CheckCurrentValuebuffer(function_name)) {
    return false;
  }
  if (!state_.bound_valuebuffer.get()->IsSubscribed(subscription)) {
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, function_name,
                       "valuebuffer is not subscribed");
    return false;
  }
  return true;
}
