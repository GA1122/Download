TaskQueue::TaskQueue(TaskQueueManager* task_queue_manager,
                     const char* disabled_by_default_tracing_category)
    : thread_id_(base::PlatformThread::CurrentId()),
      task_queue_manager_(task_queue_manager),
      pump_policy_(TaskQueueManager::PumpPolicy::AUTO),
      name_(nullptr),
      disabled_by_default_tracing_category_(
          disabled_by_default_tracing_category),
      wakeup_policy_(TaskQueueManager::WakeupPolicy::CAN_WAKE_OTHER_QUEUES) {
}
