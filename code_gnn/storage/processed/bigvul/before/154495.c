void GLES2DecoderPassthroughImpl::ReadBackBuffersIntoShadowCopies(
    const BufferShadowUpdateMap& updates) {
  if (updates.empty()) {
    return;
  }

  GLint old_binding = 0;
  api()->glGetIntegervFn(GL_ARRAY_BUFFER_BINDING, &old_binding);
  for (const auto& u : updates) {
    GLuint client_id = u.first;
    GLuint service_id = 0;
    if (!resources_->buffer_id_map.GetServiceID(client_id, &service_id)) {
      DCHECK(false);
      continue;
    }

    const auto& update = u.second;

    void* shadow = update.shm->GetDataAddress(update.shm_offset, update.size);
    DCHECK(shadow);

    api()->glBindBufferFn(GL_ARRAY_BUFFER, service_id);
    GLint already_mapped = GL_TRUE;
    api()->glGetBufferParameterivFn(GL_ARRAY_BUFFER, GL_BUFFER_MAPPED,
                                    &already_mapped);
    if (already_mapped) {
      continue;
    }

    void* mapped = api()->glMapBufferRangeFn(GL_ARRAY_BUFFER, 0, update.size,
                                             GL_MAP_READ_BIT);
    if (!mapped) {
      DLOG(ERROR) << "glMapBufferRange unexpectedly returned NULL";
      MarkContextLost(error::kOutOfMemory);
      group_->LoseContexts(error::kUnknown);
      return;
    }
    memcpy(shadow, mapped, update.size);
    bool unmap_ok = api()->glUnmapBufferFn(GL_ARRAY_BUFFER);
    if (unmap_ok == GL_FALSE) {
      DLOG(ERROR) << "glUnmapBuffer unexpectedly returned GL_FALSE";
      MarkContextLost(error::kUnknown);
      group_->LoseContexts(error::kUnknown);
      return;
    }
  }

  api()->glBindBufferFn(GL_ARRAY_BUFFER, old_binding);
}
