void GLES2Implementation::GetUniformiv(GLuint program,
                                       GLint location,
                                       GLint* params) {
  GPU_CLIENT_SINGLE_THREAD_CHECK();
  GPU_CLIENT_LOG("[" << GetLogPrefix() << "] glGetUniformiv(" << program << ", "
                     << location << ", " << static_cast<const void*>(params)
                     << ")");
  TRACE_EVENT0("gpu", "GLES2::GetUniformiv");
  typedef cmds::GetUniformiv::Result Result;
  auto result = GetResultAs<Result>();
  if (!result) {
    return;
  }
  result->SetNumResults(0);
  helper_->GetUniformiv(program, location, GetResultShmId(), result.offset());
  WaitForCmd();
  result->CopyResult(params);
  GPU_CLIENT_LOG_CODE_BLOCK({
    for (int32_t i = 0; i < result->GetNumResults(); ++i) {
      GPU_CLIENT_LOG("  " << i << ": " << result->GetData()[i]);
    }
  });
  CheckGLError();
}
