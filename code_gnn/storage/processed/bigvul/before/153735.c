bool GLES2Implementation::GetUniformIndicesHelper(GLuint program,
                                                  GLsizei count,
                                                  const char* const* names,
                                                  GLuint* indices) {
  if (!PackStringsToBucket(count, names, nullptr, "glGetUniformIndices")) {
    return false;
  }
  typedef cmds::GetUniformIndices::Result Result;
  auto result = GetResultAs<Result>();
  if (!result) {
    return false;
  }
  result->SetNumResults(0);
  helper_->GetUniformIndices(program, kResultBucketId, GetResultShmId(),
                             result.offset());
  WaitForCmd();
  if (result->GetNumResults() != count) {
    return false;
  }
  result->CopyResult(indices);
  return true;
}
