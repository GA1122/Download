void GLES2Implementation::Finish() {
  GPU_CLIENT_SINGLE_THREAD_CHECK();
  flush_id_ = GenerateNextFlushId();
  FinishHelper();
}
