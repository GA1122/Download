GLuint GLES2Implementation::GetUniformBlockIndexHelper(GLuint program,
                                                       const char* name) {
  typedef cmds::GetUniformBlockIndex::Result Result;
  SetBucketAsCString(kResultBucketId, name);
  auto result = GetResultAs<Result>();
  if (!result) {
    return GL_INVALID_INDEX;
  }
  *result = GL_INVALID_INDEX;
  helper_->GetUniformBlockIndex(program, kResultBucketId, GetResultShmId(),
                                result.offset());
  WaitForCmd();
  helper_->SetBucketSize(kResultBucketId, 0);
  return *result;
}
