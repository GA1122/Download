GLuint GLES2Implementation::GetLastFlushIdCHROMIUM() {
  GPU_CLIENT_SINGLE_THREAD_CHECK();
  GPU_CLIENT_LOG("[" << GetLogPrefix() << "] glGetLastFlushIdCHROMIUM()");
  return flush_id_;
}
