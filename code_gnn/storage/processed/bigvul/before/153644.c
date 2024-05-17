void GLES2Implementation::FlushHelper() {
  helper_->CommandBufferHelper::Flush();

  if (aggressively_free_resources_)
    FreeEverything();
}
