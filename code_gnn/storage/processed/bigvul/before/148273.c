bool PrintJobWorker::PostTask(const tracked_objects::Location& from_here,
                              const base::Closure& task) {
  if (task_runner_.get())
    return task_runner_->PostTask(from_here, task);
  return false;
}
