void PixelBufferRasterWorkerPool::Shutdown() {
  shutdown_ = true;
  RasterWorkerPool::Shutdown();
  CheckForCompletedWorkerTasks();
  CheckForCompletedUploads();
  check_for_completed_raster_tasks_callback_.Cancel();
  check_for_completed_raster_tasks_pending_ = false;
  for (TaskMap::iterator it = pixel_buffer_tasks_.begin();
       it != pixel_buffer_tasks_.end(); ++it) {
    internal::RasterWorkerPoolTask* task = it->first;
    internal::WorkerPoolTask* pixel_buffer_task = it->second.get();

    if (!pixel_buffer_task && !task->HasFinishedRunning()) {
      task->DidRun(true);
      completed_tasks_.push_back(task);
    }
  }
  DCHECK_EQ(completed_tasks_.size(), pixel_buffer_tasks_.size());
}
