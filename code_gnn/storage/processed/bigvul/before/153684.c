GLenum GLES2Implementation::GetGLError() {
  TRACE_EVENT0("gpu", "GLES2::GetGLError");
  typedef cmds::GetError::Result Result;
  auto result = GetResultAs<Result>();
  if (!result) {
    return GL_NO_ERROR;
  }
  *result = GL_NO_ERROR;
  helper_->GetError(GetResultShmId(), result.offset());
  WaitForCmd();
  GLenum error = *result;
  if (error == GL_NO_ERROR) {
    error = GetClientSideGLError();
  } else {
    error_bits_ &= ~GLES2Util::GLErrorToErrorBit(error);
  }
  return error;
}
