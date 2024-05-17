GLuint GLES2Implementation::GetMaxValueInBufferCHROMIUMHelper(GLuint buffer_id,
                                                              GLsizei count,
                                                              GLenum type,
                                                              GLuint offset) {
  typedef cmds::GetMaxValueInBufferCHROMIUM::Result Result;
  auto result = GetResultAs<Result>();
  if (!result) {
    return 0;
  }
  *result = 0;
  helper_->GetMaxValueInBufferCHROMIUM(buffer_id, count, type, offset,
                                       GetResultShmId(), result.offset());
  WaitForCmd();
  return *result;
}
