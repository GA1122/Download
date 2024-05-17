bool GLES2Implementation::GetActiveUniformBlockivHelper(GLuint program,
                                                        GLuint index,
                                                        GLenum pname,
                                                        GLint* params) {
  typedef cmds::GetActiveUniformBlockiv::Result Result;
  auto result = GetResultAs<Result>();
  if (!result) {
    return false;
  }
  result->SetNumResults(0);
  helper_->GetActiveUniformBlockiv(program, index, pname, GetResultShmId(),
                                   result.offset());
  WaitForCmd();
  if (result->GetNumResults() > 0) {
    if (params) {
      result->CopyResult(params);
    }
    GPU_CLIENT_LOG_CODE_BLOCK({
      for (int32_t i = 0; i < result->GetNumResults(); ++i) {
        GPU_CLIENT_LOG("  " << i << ": " << result->GetData()[i]);
      }
    });
    return true;
  }
  return false;
}
