const char* OptionalTaskDescriptionToString(
    base::Optional<RendererSchedulerImpl::TaskDescriptionForTracing> opt_desc) {
  if (!opt_desc)
    return nullptr;
  if (opt_desc->task_type)
    return TaskTypeToString(opt_desc->task_type.value());
  return MainThreadTaskQueue::NameForQueueType(opt_desc->queue_type);
}
