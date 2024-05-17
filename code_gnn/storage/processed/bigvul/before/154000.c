void GLES2DecoderImpl::DoFlushMappedBufferRange(
    GLenum target, GLintptr offset, GLsizeiptr size) {
  const char* func_name = "glFlushMappedBufferRange";
  if (offset < 0) {
    LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, func_name, "offset < 0");
    return;
  }
  Buffer* buffer = buffer_manager()->GetBufferInfoForTarget(&state_, target);
  if (!buffer) {
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, func_name, "no buffer bound");
    return;
  }
  const Buffer::MappedRange* mapped_range = buffer->GetMappedRange();
  if (!mapped_range) {
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, func_name, "buffer is unmapped");
    return;
  }
  if (!AllBitsSet(mapped_range->access, GL_MAP_FLUSH_EXPLICIT_BIT)) {
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, func_name,
        "buffer is mapped without MAP_FLUSH_EXPLICIT_BIT flag");
    return;
  }
  base::CheckedNumeric<int32_t> range_size = size;
  range_size += offset;
  if (!range_size.IsValid() ||
      range_size.ValueOrDefault(0) > mapped_range->size) {
    LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, func_name,
        "offset + size out of bounds");
    return;
  }
  char* client_data = reinterpret_cast<char*>(mapped_range->GetShmPointer());
  DCHECK(client_data);
  char* gpu_data = reinterpret_cast<char*>(mapped_range->pointer);
  DCHECK(gpu_data);
  memcpy(gpu_data + offset, client_data + offset, size);
  if (buffer->shadowed()) {
    buffer->SetRange(mapped_range->offset + offset, size, client_data + offset);
  }
  api()->glFlushMappedBufferRangeFn(target, offset, size);
}
