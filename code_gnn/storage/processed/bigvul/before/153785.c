bool GLES2Implementation::PackStringsToBucket(GLsizei count,
                                              const char* const* str,
                                              const GLint* length,
                                              const char* func_name) {
  DCHECK_LE(0, count);
  base::CheckedNumeric<uint32_t> total_size = count;
  total_size += 1;
  total_size *= sizeof(GLint);
  uint32_t header_size = 0;
  if (!total_size.AssignIfValid(&header_size)) {
    SetGLError(GL_INVALID_VALUE, func_name, "overflow");
    return false;
  }
  std::vector<GLint> header(count + 1);
  header[0] = static_cast<GLint>(count);
  for (GLsizei ii = 0; ii < count; ++ii) {
    GLint len = 0;
    if (str[ii]) {
      len = (length && length[ii] >= 0)
                ? length[ii]
                : base::checked_cast<GLint>(strlen(str[ii]));
    }
    total_size += len;
    total_size += 1;   
    header[ii + 1] = len;
  }
  uint32_t validated_size = 0;
  if (!total_size.AssignIfValid(&validated_size)) {
    SetGLError(GL_INVALID_VALUE, func_name, "overflow");
    return false;
  }
  helper_->SetBucketSize(kResultBucketId, validated_size);
  uint32_t offset = 0;
  for (GLsizei ii = 0; ii <= count; ++ii) {
    const char* src =
        (ii == 0) ? reinterpret_cast<const char*>(&header[0]) : str[ii - 1];
    uint32_t size = (ii == 0) ? header_size : header[ii];
    if (ii > 0) {
      size += 1;   
    }
    while (size) {
      ScopedTransferBufferPtr buffer(size, helper_, transfer_buffer_);
      if (!buffer.valid() || buffer.size() == 0) {
        SetGLError(GL_OUT_OF_MEMORY, func_name, "too large");
        return false;
      }
      uint32_t copy_size = buffer.size();
      if (ii > 0 && buffer.size() == size)
        --copy_size;
      if (copy_size)
        memcpy(buffer.address(), src, copy_size);
      if (copy_size < buffer.size()) {
        DCHECK(copy_size + 1 == buffer.size());
        char* str = reinterpret_cast<char*>(buffer.address());
        str[copy_size] = 0;
      }
      helper_->SetBucketData(kResultBucketId, offset, buffer.size(),
                             buffer.shm_id(), buffer.offset());
      offset += buffer.size();
      src += buffer.size();
      size -= buffer.size();
    }
  }
  DCHECK_EQ(total_size.ValueOrDefault(0), offset);
  return true;
}
