bool TaskQueueManager::ProcessTaskFromWorkQueue(
    size_t queue_index,
    bool has_previous_task,
    base::PendingTask* previous_task) {
  DCHECK(main_thread_checker_.CalledOnValidThread());
  scoped_refptr<DeletionSentinel> protect(deletion_sentinel_);
  internal::TaskQueue* queue = Queue(queue_index);
  base::PendingTask pending_task = queue->TakeTaskFromWorkQueue();
  task_was_run_bitmap_ |= UINT64_C(1) << queue_index;
  if (!pending_task.nestable && main_task_runner_->IsNested()) {
    main_task_runner_->PostNonNestableTask(pending_task.posted_from,
                                           pending_task.task);
  } else {
    if (has_previous_task) {
      FOR_EACH_OBSERVER(base::MessageLoop::TaskObserver, task_observers_,
                        DidProcessTask(*previous_task));
      FOR_EACH_OBSERVER(base::MessageLoop::TaskObserver, task_observers_,
                        WillProcessTask(pending_task));
    }
    task_annotator_.RunTask("TaskQueueManager::PostTask",
                            "TaskQueueManager::RunTask", pending_task);

    if (protect->HasOneRef())
      return true;

    pending_task.task.Reset();
    *previous_task = pending_task;
  }
  return false;
}
