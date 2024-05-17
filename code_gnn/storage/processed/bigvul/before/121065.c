void PixelBufferRasterWorkerPool::CheckForCompletedTasks() {
  TRACE_EVENT0("cc", "PixelBufferRasterWorkerPool::CheckForCompletedTasks");

  RasterWorkerPool::CheckForCompletedTasks();
  CheckForCompletedUploads();
  FlushUploads();

  RasterTaskDeque completed_tasks;
  completed_tasks_.swap(completed_tasks);

  while (!completed_tasks.empty()) {
    internal::RasterWorkerPoolTask* task = completed_tasks.front().get();
    DCHECK(pixel_buffer_tasks_.find(task) != pixel_buffer_tasks_.end());

    pixel_buffer_tasks_.erase(task);

    task->WillComplete();
    task->CompleteOnOriginThread();
    task->DidComplete();

    completed_tasks.pop_front();
  }
}
