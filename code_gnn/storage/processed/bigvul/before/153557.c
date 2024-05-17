void GLES2Implementation::AllocateShadowCopiesForReadback() {
  for (auto buffer : readback_buffer_shadow_tracker_->GetUnfencedBufferList()) {
    if (!buffer) {
      continue;
    }
    int32_t shm_id = 0;
    uint32_t shm_offset = 0;
    bool already_allocated = false;
    uint32_t size = buffer->Alloc(&shm_id, &shm_offset, &already_allocated);
    if (already_allocated) {
      SendErrorMessage(
          "performance warning: READ-usage buffer was written, then "
          "fenced, but written again before being read back. This discarded "
          "the shadow copy that was created to accelerate readback.",
          0);
    }
    helper_->SetReadbackBufferShadowAllocationINTERNAL(buffer->id(), shm_id,
                                                       shm_offset, size);
  }
}
