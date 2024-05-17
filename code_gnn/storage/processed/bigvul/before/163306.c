RenderThreadImpl* RenderThreadImpl::Create(
    const InProcessChildThreadParams& params) {
  std::unique_ptr<blink::scheduler::RendererScheduler> renderer_scheduler =
      blink::scheduler::RendererScheduler::Create();
  scoped_refptr<base::SingleThreadTaskRunner> test_task_counter;
  return new RenderThreadImpl(
      params, std::move(renderer_scheduler), test_task_counter);
}
