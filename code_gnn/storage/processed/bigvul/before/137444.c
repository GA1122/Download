bool MessageLoop::IsIdleForTesting() {
  return incoming_task_queue_->IsIdleForTesting();
}
