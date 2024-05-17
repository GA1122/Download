scoped_refptr<gpu::Buffer> GLES2DecoderImpl::GetShmBuffer(uint32_t shm_id) {
  return GetSharedMemoryBuffer(shm_id);
}
