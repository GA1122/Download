 PixelBufferRasterWorkerPool::~PixelBufferRasterWorkerPool() {
  DCHECK(shutdown_);
  DCHECK(!check_for_completed_raster_tasks_pending_);
  DCHECK_EQ(0u, pixel_buffer_tasks_.size());
  DCHECK_EQ(0u, tasks_with_pending_upload_.size());
  DCHECK_EQ(0u, completed_tasks_.size());
}
