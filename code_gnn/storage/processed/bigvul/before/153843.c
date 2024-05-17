void GLES2Implementation::UnmapBufferSubDataCHROMIUM(const void* mem) {
  GPU_CLIENT_SINGLE_THREAD_CHECK();
  GPU_CLIENT_LOG("[" << GetLogPrefix() << "] glUnmapBufferSubDataCHROMIUM("
                     << mem << ")");
  MappedBufferMap::iterator it = mapped_buffers_.find(mem);
  if (it == mapped_buffers_.end()) {
    SetGLError(GL_INVALID_VALUE, "UnmapBufferSubDataCHROMIUM",
               "buffer not mapped");
    return;
  }
  const MappedBuffer& mb = it->second;
  helper_->BufferSubData(mb.target, mb.offset, mb.size, mb.shm_id,
                         mb.shm_offset);
  InvalidateReadbackBufferShadowDataCHROMIUM(GetBoundBufferHelper(mb.target));
  mapped_memory_->FreePendingToken(mb.shm_memory, helper_->InsertToken());
  mapped_buffers_.erase(it);
  CheckGLError();
}
