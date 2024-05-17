void GLES2DecoderPassthroughImpl::InsertError(GLenum error,
                                              const std::string& message) {
  errors_.insert(error);
  LogGLDebugMessage(GL_DEBUG_SOURCE_API, GL_DEBUG_TYPE_ERROR, error,
                    GL_DEBUG_SEVERITY_HIGH, message.length(), message.c_str(),
                    GetLogger());
}
