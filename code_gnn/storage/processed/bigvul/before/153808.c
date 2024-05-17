void GLES2Implementation::SetAggressivelyFreeResources(
    bool aggressively_free_resources) {
  TRACE_EVENT1("gpu", "GLES2Implementation::SetAggressivelyFreeResources",
               "aggressively_free_resources", aggressively_free_resources);
  aggressively_free_resources_ = aggressively_free_resources;

  if (aggressively_free_resources_ && helper_->HaveRingBuffer()) {
    FlushDriverCachesCHROMIUM();

    Flush();
  } else {
    ShallowFlushCHROMIUM();
  }
}
