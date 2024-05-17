GLint GLES2Implementation::GetFragDataIndexEXTHelper(GLuint program,
                                                     const char* name) {
  typedef cmds::GetFragDataIndexEXT::Result Result;
  SetBucketAsCString(kResultBucketId, name);
  auto result = GetResultAs<Result>();
  if (!result) {
    return -1;
  }
  *result = -1;
  helper_->GetFragDataIndexEXT(program, kResultBucketId, GetResultShmId(),
                               result.offset());
  WaitForCmd();
  helper_->SetBucketSize(kResultBucketId, 0);
  return *result;
}
