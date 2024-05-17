void GLES2Implementation::BindUniformLocationCHROMIUM(GLuint program,
                                                      GLint location,
                                                      const char* name) {
  GPU_CLIENT_SINGLE_THREAD_CHECK();
  GPU_CLIENT_LOG("[" << GetLogPrefix() << "] glBindUniformLocationCHROMIUM("
                     << program << ", " << location << ", " << name << ")");
  SetBucketAsString(kResultBucketId, name);
  helper_->BindUniformLocationCHROMIUMBucket(program, location,
                                             kResultBucketId);
  helper_->SetBucketSize(kResultBucketId, 0);
  CheckGLError();
}
