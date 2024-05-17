scoped_refptr<base::SingleThreadTaskRunner> WebLocalFrameImpl::GetTaskRunner(
    TaskType task_type) {
  return GetFrame()->GetTaskRunner(task_type);
}
