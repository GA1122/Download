void GLES2Implementation::GetInternalformativ(GLenum target,
                                              GLenum format,
                                              GLenum pname,
                                              GLsizei buf_size,
                                              GLint* params) {
  GPU_CLIENT_SINGLE_THREAD_CHECK();
  GPU_CLIENT_VALIDATE_DESTINATION_INITALIZATION(GLint, params);
  GPU_CLIENT_LOG("[" << GetLogPrefix() << "] glGetInternalformativ("
                     << GLES2Util::GetStringRenderBufferTarget(target) << ", "
                     << GLES2Util::GetStringRenderBufferFormat(format) << ", "
                     << GLES2Util::GetStringInternalFormatParameter(pname)
                     << ", " << buf_size << ", "
                     << static_cast<const void*>(params) << ")");
  if (buf_size < 0) {
    SetGLError(GL_INVALID_VALUE, "glGetInternalformativ", "bufSize < 0");
    return;
  }
  TRACE_EVENT0("gpu", "GLES2Implementation::GetInternalformativ");
  if (GetInternalformativHelper(target, format, pname, buf_size, params)) {
    return;
  }
  typedef cmds::GetInternalformativ::Result Result;
  auto result = GetResultAs<Result>();
  if (!result) {
    return;
  }
  result->SetNumResults(0);
  helper_->GetInternalformativ(target, format, pname, GetResultShmId(),
                               result.offset());
  WaitForCmd();
  GPU_CLIENT_LOG_CODE_BLOCK({
    for (int32_t i = 0; i < result->GetNumResults(); ++i) {
      GPU_CLIENT_LOG("  " << i << ": " << result->GetData()[i]);
    }
  });
  if (buf_size > 0 && params) {
    GLint* data = result->GetData();
    if (buf_size >= result->GetNumResults()) {
      buf_size = result->GetNumResults();
    }
    for (GLsizei ii = 0; ii < buf_size; ++ii) {
      params[ii] = data[ii];
    }
  }
  CheckGLError();
}
