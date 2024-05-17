void TaskQueueManager::DoWork(bool posted_from_main_thread) {
  if (posted_from_main_thread) {
    pending_dowork_count_--;
    DCHECK_GE(pending_dowork_count_, 0);
  }
  DCHECK(main_thread_checker_.CalledOnValidThread());

  if (!UpdateWorkQueues(false, nullptr))
    return;

  base::PendingTask previous_task((tracked_objects::Location()),
                                  (base::Closure()));
  for (int i = 0; i < work_batch_size_; i++) {
    size_t queue_index;
    if (!SelectWorkQueueToService(&queue_index))
      return;
    MaybePostDoWorkOnMainRunner();

    if (ProcessTaskFromWorkQueue(queue_index, i > 0, &previous_task))
      return;   

    bool should_trigger_wakeup = Queue(queue_index)->wakeup_policy() ==
                                 WakeupPolicy::CAN_WAKE_OTHER_QUEUES;
    if (!UpdateWorkQueues(should_trigger_wakeup, &previous_task))
      return;
  }
}
