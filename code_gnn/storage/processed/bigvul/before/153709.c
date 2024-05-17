bool GLES2Implementation::GetProgramResourceivHelper(GLuint program,
                                                     GLenum program_interface,
                                                     GLuint index,
                                                     GLsizei prop_count,
                                                     const GLenum* props,
                                                     GLsizei bufsize,
                                                     GLsizei* length,
                                                     GLint* params) {
  DCHECK_LE(0, prop_count);
  DCHECK_LE(0, bufsize);
  base::CheckedNumeric<uint32_t> bytes = prop_count;
  bytes *= sizeof(GLenum);
  if (!bytes.IsValid()) {
    SetGLError(GL_INVALID_VALUE, "glGetProgramResourceiv", "count overflow");
    return false;
  }
  SetBucketContents(kResultBucketId, props, bytes.ValueOrDefault(0));
  typedef cmds::GetProgramResourceiv::Result Result;
  auto result = GetResultAs<Result>();
  if (!result) {
    return false;
  }
  result->SetNumResults(0);
  helper_->GetProgramResourceiv(program, program_interface, index,
                                kResultBucketId, GetResultShmId(),
                                result.offset());
  WaitForCmd();
  if (length) {
    *length = result->GetNumResults();
  }
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
