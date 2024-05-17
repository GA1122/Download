  static IOSurfaceGpuMemoryBuffer* FromClientBuffer(ClientBuffer buffer) {
    return reinterpret_cast<IOSurfaceGpuMemoryBuffer*>(buffer);
  }
