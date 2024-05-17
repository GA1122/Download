void GLES2DecoderImpl::SetGLError(
    GLenum error, const char* function_name, const char* msg) {
  if (msg) {
    last_error_ = msg;
    LogMessage(GetLogPrefix() + ": " + std::string("GL ERROR :") +
               GLES2Util::GetStringEnum(error) + " : " +
               function_name + ": " + msg);
  }
  error_bits_ |= GLES2Util::GLErrorToErrorBit(error);
}
