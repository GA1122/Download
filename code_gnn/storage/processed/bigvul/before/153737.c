void GLES2Implementation::GetUniformfv(GLuint program,
                                       GLint location,
                                       GLfloat* params) {
  GPU_CLIENT_SINGLE_THREAD_CHECK();
  GPU_CLIENT_LOG("[" << GetLogPrefix() << "] glGetUniformfv(" << program << ", "
                     << location << ", " << static_cast<const void*>(params)
                     << ")");
  TRACE_EVENT0("gpu", "GLES2::GetUniformfv");
  typedef cmds::GetUniformfv::Result Result;
  auto result = GetResultAs<Result>();
  if (!result) {
    return;
  }
  result->SetNumResults(0);
  helper_->GetUniformfv(program, location, GetResultShmId(), result.offset());
  WaitForCmd();
  result->CopyResult(params);
  GPU_CLIENT_LOG_CODE_BLOCK({
    for (int32_t i = 0; i < result->GetNumResults(); ++i) {
      GPU_CLIENT_LOG("  " << i << ": " << result->GetData()[i]);
    }
  });
  CheckGLError();
}
