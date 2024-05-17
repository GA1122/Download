void MessageLoop::AddTaskObserver(TaskObserver* task_observer) {
  DCHECK_EQ(this, current());
  CHECK(allow_task_observers_);
  task_observers_.AddObserver(task_observer);
}
