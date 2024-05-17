void GLES2Implementation::GetAttachedShaders(GLuint program,
                                             GLsizei maxcount,
                                             GLsizei* count,
                                             GLuint* shaders) {
  GPU_CLIENT_SINGLE_THREAD_CHECK();
  GPU_CLIENT_LOG("[" << GetLogPrefix() << "] glGetAttachedShaders(" << program
                     << ", " << maxcount << ", "
                     << static_cast<const void*>(count) << ", "
                     << static_cast<const void*>(shaders) << ", ");
  if (maxcount < 0) {
    SetGLError(GL_INVALID_VALUE, "glGetAttachedShaders", "maxcount < 0");
    return;
  }
  TRACE_EVENT0("gpu", "GLES2::GetAttachedShaders");
  typedef cmds::GetAttachedShaders::Result Result;
  uint32_t checked_size = 0;
  if (!Result::ComputeSize(maxcount).AssignIfValid(&checked_size)) {
    SetGLError(GL_OUT_OF_MEMORY, "glGetAttachedShaders",
               "allocation too large");
    return;
  }
  Result* result = static_cast<Result*>(transfer_buffer_->Alloc(checked_size));
  if (!result) {
    return;
  }
  result->SetNumResults(0);
  helper_->GetAttachedShaders(program, transfer_buffer_->GetShmId(),
                              transfer_buffer_->GetOffset(result),
                              checked_size);
  int32_t token = helper_->InsertToken();
  WaitForCmd();
  if (count) {
    *count = result->GetNumResults();
  }
  result->CopyResult(shaders);
  GPU_CLIENT_LOG_CODE_BLOCK({
    for (int32_t i = 0; i < result->GetNumResults(); ++i) {
      GPU_CLIENT_LOG("  " << i << ": " << result->GetData()[i]);
    }
  });
  transfer_buffer_->FreePendingToken(result, token);
  CheckGLError();
}
