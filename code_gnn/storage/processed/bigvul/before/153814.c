void GLES2Implementation::ShallowFinishCHROMIUM() {
  GPU_CLIENT_SINGLE_THREAD_CHECK();
  TRACE_EVENT0("gpu", "GLES2::ShallowFinishCHROMIUM");
  flush_id_ = GenerateNextFlushId();
  helper_->CommandBufferHelper::Finish();

  if (aggressively_free_resources_)
    FreeEverything();
}
