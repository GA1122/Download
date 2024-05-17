MessageLoop::~MessageLoop() {
  DCHECK((pump_ && current() == this) || (!pump_ && current() != this));

#if !defined(OS_IOS)
  DCHECK((!pump_ && current() != this) || !RunLoop::IsRunningOnCurrentThread());
#endif

#if defined(OS_WIN)
  if (in_high_res_mode_)
    Time::ActivateHighResolutionTimer(false);
#endif
  bool did_work;
  for (int i = 0; i < 100; ++i) {
    DeletePendingTasks();
    ReloadWorkQueue();
    did_work = DeletePendingTasks();
    if (!did_work)
      break;
  }
  DCHECK(!did_work);

  for (auto& observer : destruction_observers_)
    observer.WillDestroyCurrentMessageLoop();

  thread_task_runner_handle_.reset();

  incoming_task_queue_->WillDestroyCurrentMessageLoop();
  incoming_task_queue_ = NULL;
  unbound_task_runner_ = NULL;
  task_runner_ = NULL;

  if (current() == this)
    GetTLSMessageLoop()->Set(nullptr);
}
