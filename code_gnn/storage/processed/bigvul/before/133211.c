void TaskQueueManager::AddTaskObserver(
    base::MessageLoop::TaskObserver* task_observer) {
  DCHECK(main_thread_checker_.CalledOnValidThread());
  main_task_runner_->AddTaskObserver(task_observer);
  task_observers_.AddObserver(task_observer);
}
