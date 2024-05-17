error::Error GLES2DecoderImpl::HandleUnmapBuffer(
    uint32_t immediate_data_size, const void* cmd_data) {
  if (!unsafe_es3_apis_enabled()) {
    return error::kUnknownCommand;
  }
  const gles2::cmds::UnmapBuffer& c =
      *static_cast<const gles2::cmds::UnmapBuffer*>(cmd_data);
  GLenum target = static_cast<GLenum>(c.target);

  Buffer* buffer = buffer_manager()->GetBufferInfoForTarget(&state_, target);
  if (!buffer) {
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, "UnmapBuffer", "no buffer bound");
    return error::kNoError;
  }
  const Buffer::MappedRange* mapped_range = buffer->GetMappedRange();
  if (!mapped_range) {
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, "UnmapBuffer",
                       "buffer is unmapped");
    return error::kNoError;
  }
  if ((mapped_range->access & GL_MAP_WRITE_BIT) == 0 ||
      (mapped_range->access & GL_MAP_FLUSH_EXPLICIT_BIT) ==
           GL_MAP_FLUSH_EXPLICIT_BIT) {
  } else {
    void* mem = mapped_range->GetShmPointer();
    if (!mem) {
      return error::kOutOfBounds;
    }
    DCHECK(mapped_range->pointer);
    memcpy(mapped_range->pointer, mem, mapped_range->size);
  }
  buffer->RemoveMappedRange();
  GLboolean rt = glUnmapBuffer(target);
  if (rt == GL_FALSE) {
    LOG(ERROR) << "glUnmapBuffer unexpectedly returned GL_FALSE";
    group_->LoseContexts(GL_INNOCENT_CONTEXT_RESET_ARB);
    reset_status_ = GL_GUILTY_CONTEXT_RESET_ARB;
    return error::kLostContext;
  }
  return error::kNoError;
}
