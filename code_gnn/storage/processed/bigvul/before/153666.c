bool GLES2Implementation::GetActiveUniformHelper(GLuint program,
                                                 GLuint index,
                                                 GLsizei bufsize,
                                                 GLsizei* length,
                                                 GLint* size,
                                                 GLenum* type,
                                                 char* name) {
  helper_->SetBucketSize(kResultBucketId, 0);
  typedef cmds::GetActiveUniform::Result Result;
  auto result = GetResultAs<Result>();
  if (!result) {
    return false;
  }
  result->success = false;
  helper_->GetActiveUniform(program, index, kResultBucketId, GetResultShmId(),
                            result.offset());
  WaitForCmd();
  bool success = !!result->success;
  if (success) {
    if (size) {
      *size = result->size;
    }
    if (type) {
      *type = result->type;
    }
    GetResultNameHelper(bufsize, length, name);
  }
  return success;
}
