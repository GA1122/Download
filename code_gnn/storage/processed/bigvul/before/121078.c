const char* PixelBufferRasterWorkerPool::StateName() const {
  if (scheduled_raster_task_count_)
    return "rasterizing";
  if (PendingRasterTaskCount())
    return "throttled";
  if (!tasks_with_pending_upload_.empty())
    return "waiting_for_uploads";

  return "finishing";
}
