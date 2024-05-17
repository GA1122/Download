bool GLES2Implementation::GetTransformFeedbackVaryingHelper(GLuint program,
                                                            GLuint index,
                                                            GLsizei bufsize,
                                                            GLsizei* length,
                                                            GLint* size,
                                                            GLenum* type,
                                                            char* name) {
  helper_->SetBucketSize(kResultBucketId, 0);
  typedef cmds::GetTransformFeedbackVarying::Result Result;
  auto result = GetResultAs<Result>();
  if (!result) {
    return false;
  }
  result->success = false;
  helper_->GetTransformFeedbackVarying(program, index, kResultBucketId,
                                       GetResultShmId(), result.offset());
  WaitForCmd();
  if (result->success) {
    if (size) {
      *size = result->size;
    }
    if (type) {
      *type = result->type;
    }
    GetResultNameHelper(bufsize, length, name);
  }
  return result->success != 0;
}
