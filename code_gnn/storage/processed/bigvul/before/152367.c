scoped_refptr<base::SingleThreadTaskRunner> RenderFrameImpl::GetTaskRunner(
    blink::TaskType task_type) {
  return GetWebFrame()->GetTaskRunner(task_type);
}
