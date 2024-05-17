void GLES2Implementation::BindFragmentInputLocationCHROMIUM(GLuint program,
                                                            GLint location,
                                                            const char* name) {
  GPU_CLIENT_SINGLE_THREAD_CHECK();
  GPU_CLIENT_LOG("[" << GetLogPrefix()
                     << "] glBindFragmentInputLocationCHROMIUM(" << program
                     << ", " << location << ", " << name << ")");
  SetBucketAsString(kResultBucketId, name);
  helper_->BindFragmentInputLocationCHROMIUMBucket(program, location,
                                                   kResultBucketId);
  helper_->SetBucketSize(kResultBucketId, 0);
  CheckGLError();
}
