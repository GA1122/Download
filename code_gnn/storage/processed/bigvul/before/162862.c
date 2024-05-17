  IOSurfaceGpuMemoryBuffer(const gfx::Size& size, gfx::BufferFormat format)
      : mapped_(false), size_(size), format_(format) {
    iosurface_ = gfx::CreateIOSurface(size, gfx::BufferFormat::BGRA_8888);
  }
