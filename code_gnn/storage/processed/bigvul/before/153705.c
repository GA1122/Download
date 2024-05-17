GLint GLES2Implementation::GetProgramResourceLocationHelper(
    GLuint program,
    GLenum program_interface,
    const char* name) {
  typedef cmds::GetProgramResourceLocation::Result Result;
  SetBucketAsCString(kResultBucketId, name);
  auto result = GetResultAs<Result>();
  if (!result) {
    return -1;
  }
  *result = -1;
  helper_->GetProgramResourceLocation(program, program_interface,
                                      kResultBucketId, GetResultShmId(),
                                      result.offset());
  WaitForCmd();
  helper_->SetBucketSize(kResultBucketId, 0);
  return *result;
}
