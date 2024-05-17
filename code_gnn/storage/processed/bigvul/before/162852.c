  static GpuMemoryBufferImpl* FromClientBuffer(ClientBuffer buffer) {
    return reinterpret_cast<GpuMemoryBufferImpl*>(buffer);
  }
