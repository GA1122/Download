void GLES2Implementation::BindFragDataLocationIndexedEXT(GLuint program,
                                                         GLuint colorName,
                                                         GLuint index,
                                                         const char* name) {
  GPU_CLIENT_SINGLE_THREAD_CHECK();
  GPU_CLIENT_LOG("[" << GetLogPrefix() << "] glBindFragDataLocationEXT("
                     << program << ", " << colorName << ", " << index << ", "
                     << name << ")");
  SetBucketAsString(kResultBucketId, name);
  helper_->BindFragDataLocationIndexedEXTBucket(program, colorName, index,
                                                kResultBucketId);
  helper_->SetBucketSize(kResultBucketId, 0);
  CheckGLError();
}
