void GLES2Implementation::GetVertexAttribIuiv(GLuint index,
                                              GLenum pname,
                                              GLuint* params) {
  GPU_CLIENT_SINGLE_THREAD_CHECK();
  GPU_CLIENT_LOG("[" << GetLogPrefix() << "] glGetVertexAttribIuiv(" << index
                     << ", " << GLES2Util::GetStringVertexAttribute(pname)
                     << ", " << static_cast<const void*>(params) << ")");
  uint32_t value = 0;
  if (vertex_array_object_manager_->GetVertexAttrib(index, pname, &value)) {
    *params = static_cast<GLuint>(value);
    return;
  }
  TRACE_EVENT0("gpu", "GLES2::GetVertexAttribIuiv");
  typedef cmds::GetVertexAttribiv::Result Result;
  auto result = GetResultAs<Result>();
  if (!result) {
    return;
  }
  result->SetNumResults(0);
  helper_->GetVertexAttribIuiv(index, pname, GetResultShmId(), result.offset());
  WaitForCmd();
  result->CopyResult(params);
  GPU_CLIENT_LOG_CODE_BLOCK({
    for (int32_t i = 0; i < result->GetNumResults(); ++i) {
      GPU_CLIENT_LOG("  " << i << ": " << result->GetData()[i]);
    }
  });
  CheckGLError();
}
