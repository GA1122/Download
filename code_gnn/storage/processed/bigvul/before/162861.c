  GpuMemoryBufferImpl(base::RefCountedBytes* bytes,
                      const gfx::Size& size,
                      gfx::BufferFormat format)
      : mapped_(false), bytes_(bytes), size_(size), format_(format) {}
