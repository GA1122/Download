GLint GLES2Implementation::GetFragDataLocationHelper(GLuint program,
                                                     const char* name) {
  typedef cmds::GetFragDataLocation::Result Result;
  SetBucketAsCString(kResultBucketId, name);
  auto result = GetResultAs<Result>();
  if (!result) {
    return -1;
  }
  *result = -1;
  helper_->GetFragDataLocation(program, kResultBucketId, GetResultShmId(),
                               result.offset());
  WaitForCmd();
  helper_->SetBucketSize(kResultBucketId, 0);
  return *result;
}
