void MessageLoop::ReloadWorkQueue() {
  if (work_queue_.empty()) {
#if defined(OS_WIN)
    pending_high_res_tasks_ +=
        incoming_task_queue_->ReloadWorkQueue(&work_queue_);
#else
    incoming_task_queue_->ReloadWorkQueue(&work_queue_);
#endif
  }
}
