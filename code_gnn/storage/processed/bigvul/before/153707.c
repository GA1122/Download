bool GLES2Implementation::GetProgramResourceNameHelper(GLuint program,
                                                       GLenum program_interface,
                                                       GLuint index,
                                                       GLsizei bufsize,
                                                       GLsizei* length,
                                                       char* name) {
  DCHECK_LE(0, bufsize);
  helper_->SetBucketSize(kResultBucketId, 0);
  bool success = false;
  {
    typedef cmds::GetProgramResourceName::Result Result;
    auto result = GetResultAs<Result>();
    if (!result) {
      return false;
    }
    *result = 0;
    helper_->GetProgramResourceName(program, program_interface, index,
                                    kResultBucketId, GetResultShmId(),
                                    result.offset());
    WaitForCmd();
    success = !!*result;
  }
  if (success) {
    GetResultNameHelper(bufsize, length, name);
  }
  return success;
}
