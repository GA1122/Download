void GLES2Implementation::GetVertexAttribPointerv(GLuint index,
                                                  GLenum pname,
                                                  void** ptr) {
  GPU_CLIENT_SINGLE_THREAD_CHECK();
  GPU_CLIENT_LOG("[" << GetLogPrefix() << "] glGetVertexAttribPointer(" << index
                     << ", " << GLES2Util::GetStringVertexPointer(pname) << ", "
                     << static_cast<void*>(ptr) << ")");
  GPU_CLIENT_LOG_CODE_BLOCK(int32_t num_results = 1);
  if (!vertex_array_object_manager_->GetAttribPointer(index, pname, ptr)) {
    TRACE_EVENT0("gpu", "GLES2::GetVertexAttribPointerv");
    typedef cmds::GetVertexAttribPointerv::Result Result;
    auto result = GetResultAs<Result>();
    if (!result) {
      return;
    }
    result->SetNumResults(0);
    helper_->GetVertexAttribPointerv(index, pname, GetResultShmId(),
                                     result.offset());
    WaitForCmd();
    result->CopyResult(ptr);
    GPU_CLIENT_LOG_CODE_BLOCK(num_results = result->GetNumResults());
  }
  GPU_CLIENT_LOG_CODE_BLOCK({
    for (int32_t i = 0; i < num_results; ++i) {
      GPU_CLIENT_LOG("  " << i << ": " << ptr[i]);
    }
  });
  CheckGLError();
}
