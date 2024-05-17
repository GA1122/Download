void PixelBufferRasterWorkerPool::ScheduleTasks(RasterTask::Queue* queue) {
  TRACE_EVENT0("cc", "PixelBufferRasterWorkerPool::ScheduleTasks");

  RasterWorkerPool::SetRasterTasks(queue);

  if (!should_notify_client_if_no_tasks_are_pending_)
    TRACE_EVENT_ASYNC_BEGIN0("cc", "ScheduledTasks", this);

  should_notify_client_if_no_tasks_are_pending_ = true;
  should_notify_client_if_no_tasks_required_for_activation_are_pending_ = true;

  tasks_required_for_activation_.clear();

  TaskMap new_pixel_buffer_tasks;
  RasterTaskVector gpu_raster_tasks;
  for (RasterTaskVector::const_iterator it = raster_tasks().begin();
       it != raster_tasks().end(); ++it) {
    internal::RasterWorkerPoolTask* task = it->get();
    DCHECK(new_pixel_buffer_tasks.find(task) == new_pixel_buffer_tasks.end());
    DCHECK(!task->HasCompleted());
    DCHECK(!task->WasCanceled());

    if (task->use_gpu_rasterization()) {
      gpu_raster_tasks.push_back(task);
      continue;
    }

    new_pixel_buffer_tasks[task] = pixel_buffer_tasks_[task];
    pixel_buffer_tasks_.erase(task);

    if (IsRasterTaskRequiredForActivation(task))
      tasks_required_for_activation_.insert(task);
  }

  for (TaskMap::iterator it = pixel_buffer_tasks_.begin();
       it != pixel_buffer_tasks_.end(); ++it) {
    internal::RasterWorkerPoolTask* task = it->first;
    internal::WorkerPoolTask* pixel_buffer_task = it->second.get();

    new_pixel_buffer_tasks[task] = pixel_buffer_task;

    if (!pixel_buffer_task && !task->HasFinishedRunning()) {
      task->DidRun(true);
      DCHECK(std::find(completed_tasks_.begin(),
                       completed_tasks_.end(),
                       task) == completed_tasks_.end());
      completed_tasks_.push_back(task);
    } else if (IsRasterTaskRequiredForActivation(task)) {
      tasks_required_for_activation_.insert(task);
    }
  }

  for (RasterTaskDeque::const_iterator it = completed_tasks_.begin();
       it != completed_tasks_.end() && !tasks_required_for_activation_.empty();
       ++it) {
    tasks_required_for_activation_.erase(*it);
  }

  pixel_buffer_tasks_.swap(new_pixel_buffer_tasks);

  CheckForCompletedWorkerTasks();
  CheckForCompletedUploads();
  FlushUploads();

  ScheduleMoreTasks();

  check_for_completed_raster_tasks_callback_.Cancel();
  check_for_completed_raster_tasks_pending_ = false;
  ScheduleCheckForCompletedRasterTasks();

  RunGpuRasterTasks(gpu_raster_tasks);

  TRACE_EVENT_ASYNC_STEP_INTO1(
      "cc", "ScheduledTasks", this, StateName(),
      "state", TracedValue::FromValue(StateAsValue().release()));
}
