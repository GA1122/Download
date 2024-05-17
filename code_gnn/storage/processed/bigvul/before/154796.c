GLES2DecoderPassthroughImpl::DoSetReadbackBufferShadowAllocationINTERNAL(
    GLuint buffer_id,
    GLuint shm_id,
    GLuint shm_offset,
    GLuint size) {
  BufferShadowUpdate update;
  update.shm = GetSharedMemoryBuffer(shm_id);
  update.shm_offset = shm_offset;
  update.size = size;

  GLuint buffer_service_id = 0;
  if (!resources_->buffer_id_map.GetServiceID(buffer_id, &buffer_service_id)) {
    InsertError(GL_INVALID_OPERATION, "Invalid buffer ID");
    return error::kNoError;
  }

  if (!update.shm) {
    return error::kInvalidArguments;
  }
  if (update.shm->GetRemainingSize(shm_offset) < size) {
    return error::kOutOfBounds;
  }

  buffer_shadow_updates_.emplace(buffer_id, std::move(update));

  return error::kNoError;
}
