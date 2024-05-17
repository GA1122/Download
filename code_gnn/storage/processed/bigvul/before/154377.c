bool GLES2DecoderImpl::UnmapBufferHelper(Buffer* buffer, GLenum target) {
  DCHECK(buffer);
  const Buffer::MappedRange* mapped_range = buffer->GetMappedRange();
  if (!mapped_range)
    return true;
  if (!AllBitsSet(mapped_range->access, GL_MAP_WRITE_BIT) ||
      AllBitsSet(mapped_range->access, GL_MAP_FLUSH_EXPLICIT_BIT)) {
  } else if (!WasContextLost()) {
    void* mem = mapped_range->GetShmPointer();
    DCHECK(mem);
    DCHECK(mapped_range->pointer);
    memcpy(mapped_range->pointer, mem, mapped_range->size);
    if (buffer->shadowed()) {
      buffer->SetRange(mapped_range->offset, mapped_range->size, mem);
    }
  }
  buffer->RemoveMappedRange();
  if (WasContextLost())
    return true;
  GLboolean rt = api()->glUnmapBufferFn(target);
  if (rt == GL_FALSE) {
    LOG(ERROR) << "glUnmapBuffer unexpectedly returned GL_FALSE";
    MarkContextLost(error::kGuilty);
    group_->LoseContexts(error::kInnocent);
    return false;
  }
  return true;
}
