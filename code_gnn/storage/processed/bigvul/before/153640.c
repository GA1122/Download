void GLES2Implementation::EndQueryEXT(GLenum target) {
  QueryTracker::Query* query = nullptr;
  {
    GPU_CLIENT_SINGLE_THREAD_CHECK();
    GPU_CLIENT_LOG("[" << GetLogPrefix() << "] EndQueryEXT("
                       << GLES2Util::GetStringQueryTarget(target) << ")");
    query = query_tracker_->GetCurrentQuery(target);
    if (!query_tracker_->EndQuery(target, this)) {
      return;
    }
    CheckGLError();
  }   

  if (target == GL_READBACK_SHADOW_COPIES_UPDATED_CHROMIUM) {
    DCHECK(capabilities_.chromium_nonblocking_readback);
    DCHECK(query);
    auto serial = readback_buffer_shadow_tracker_->buffer_shadow_serial();
    readback_buffer_shadow_tracker_->IncrementSerial();
    auto buffers = readback_buffer_shadow_tracker_->TakeUnfencedBufferList();
    query->SetCompletedCallback(
        base::BindOnce(&GLES2Implementation::BufferShadowWrittenCallback,
                       std::move(buffers), serial));
  }
}
