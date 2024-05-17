  PixelBufferWorkerPoolTaskImpl(internal::RasterWorkerPoolTask* task,
                                uint8_t* buffer,
                                const Reply& reply)
      : task_(task),
        buffer_(buffer),
        reply_(reply),
        needs_upload_(false) {
  }
