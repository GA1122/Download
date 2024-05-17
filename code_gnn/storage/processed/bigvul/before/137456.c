void MessageLoop::RemoveTaskObserver(TaskObserver* task_observer) {
  DCHECK_EQ(this, current());
  CHECK(allow_task_observers_);
  task_observers_.RemoveObserver(task_observer);
}
