void* GLES2Implementation::MapBufferRange(GLenum target,
                                          GLintptr offset,
                                          GLsizeiptr size,
                                          GLbitfield access) {
  GPU_CLIENT_SINGLE_THREAD_CHECK();
  GPU_CLIENT_LOG("[" << GetLogPrefix() << "] glMapBufferRange("
                     << GLES2Util::GetStringEnum(target) << ", " << offset
                     << ", " << size << ", " << access << ")");
  if (!ValidateSize("glMapBufferRange", size) ||
      !ValidateOffset("glMapBufferRange", offset)) {
    return nullptr;
  }

  GLuint buffer = GetBoundBufferHelper(target);

  void* mem = nullptr;

  if (access == GL_MAP_READ_BIT) {
    if (auto* buffer_object =
            readback_buffer_shadow_tracker_->GetBuffer(buffer)) {
      mem = buffer_object->MapReadbackShm(offset, size);
      if (!mem) {
        SendErrorMessage(
            "performance warning: READ-usage buffer was read back without "
            "waiting on a fence. This caused a graphics pipeline stall.",
            0);
      }
    }
  }

  int32_t shm_id = 0;
  unsigned int shm_offset = 0;
  if (!mem) {
    mem = mapped_memory_->Alloc(size, &shm_id, &shm_offset);
    if (!mem) {
      SetGLError(GL_OUT_OF_MEMORY, "glMapBufferRange", "out of memory");
      return nullptr;
    }

    typedef cmds::MapBufferRange::Result Result;
    auto result = GetResultAs<Result>();
    *result = 0;
    helper_->MapBufferRange(target, offset, size, access, shm_id, shm_offset,
                            GetResultShmId(), result.offset());
    WaitForCmd();
    if (*result) {
      const GLbitfield kInvalidateBits =
          GL_MAP_INVALIDATE_BUFFER_BIT | GL_MAP_INVALIDATE_RANGE_BIT;
      if ((access & kInvalidateBits) != 0) {
        memset(mem, 0, size);
      }
    } else {
      mapped_memory_->Free(mem);
      mem = nullptr;
    }
  }

  if (mem) {
    DCHECK_NE(0u, buffer);
    DCHECK(mapped_buffer_range_map_.find(buffer) ==
           mapped_buffer_range_map_.end());
    auto iter = mapped_buffer_range_map_.insert(std::make_pair(
        buffer,
        MappedBuffer(access, shm_id, mem, shm_offset, target, offset, size)));
    DCHECK(iter.second);
  }

  GPU_CLIENT_LOG("  returned " << mem);
  CheckGLError();
  return mem;
}
