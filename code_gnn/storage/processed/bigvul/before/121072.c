void PixelBufferRasterWorkerPool::OnRasterTaskCompleted(
    scoped_refptr<internal::RasterWorkerPoolTask> task,
    bool was_canceled,
    bool needs_upload) {
  TRACE_EVENT2(TRACE_DISABLED_BY_DEFAULT("cc"),
               "PixelBufferRasterWorkerPool::OnRasterTaskCompleted",
               "was_canceled", was_canceled,
               "needs_upload", needs_upload);

  DCHECK(!task->use_gpu_rasterization());
  DCHECK(pixel_buffer_tasks_.find(task.get()) != pixel_buffer_tasks_.end());

  resource_provider()->UnmapPixelBuffer(task->resource()->id());

  if (!needs_upload) {
    resource_provider()->ReleasePixelBuffer(task->resource()->id());

    if (was_canceled) {
      RasterTaskVector::const_iterator it = std::find(raster_tasks().begin(),
                                                      raster_tasks().end(),
                                                      task);
      if (it != raster_tasks().end()) {
        pixel_buffer_tasks_[task.get()] = NULL;
        return;
      }
    }

    task->DidRun(was_canceled);
    DCHECK(std::find(completed_tasks_.begin(),
                     completed_tasks_.end(),
                     task) == completed_tasks_.end());
    completed_tasks_.push_back(task);
    tasks_required_for_activation_.erase(task);
    return;
  }

  DCHECK(!was_canceled);

  resource_provider()->BeginSetPixels(task->resource()->id());
  has_performed_uploads_since_last_flush_ = true;

  bytes_pending_upload_ += task->resource()->bytes();
  tasks_with_pending_upload_.push_back(task);
}
