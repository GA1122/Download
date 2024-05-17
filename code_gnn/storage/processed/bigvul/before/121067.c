void PixelBufferRasterWorkerPool::FlushUploads() {
  if (!has_performed_uploads_since_last_flush_)
    return;

  resource_provider()->ShallowFlushIfSupported();
  has_performed_uploads_since_last_flush_ = false;
}
