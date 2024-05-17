std::unique_ptr<gfx::GpuMemoryBuffer> GLManager::CreateGpuMemoryBuffer(
    const gfx::Size& size,
    gfx::BufferFormat format) {
#if defined(OS_MACOSX)
  if (use_iosurface_memory_buffers_) {
    return base::WrapUnique<gfx::GpuMemoryBuffer>(
        new IOSurfaceGpuMemoryBuffer(size, format));
  }
#endif   
  std::vector<uint8_t> data(gfx::BufferSizeForBufferFormat(size, format), 0);
  scoped_refptr<base::RefCountedBytes> bytes(new base::RefCountedBytes(data));
  return base::WrapUnique<gfx::GpuMemoryBuffer>(
      new GpuMemoryBufferImpl(bytes.get(), size, format));
}
