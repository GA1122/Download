WebGLRenderingContextBase::LRUImageBufferCache::LRUImageBufferCache(
    int capacity)
    : buffers_(WrapArrayUnique(new std::unique_ptr<ImageBuffer>[capacity])),
      capacity_(capacity) {}
