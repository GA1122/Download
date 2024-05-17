RenderThreadImpl::RenderThreadImpl(
    const InProcessChildThreadParams& params,
    std::unique_ptr<blink::scheduler::RendererScheduler> scheduler,
    const scoped_refptr<base::SingleThreadTaskRunner>& resource_task_queue)
    : ChildThreadImpl(Options::Builder()
                          .InBrowserProcess(params)
                          .AutoStartServiceManagerConnection(false)
                          .ConnectToBrowser(true)
                          .Build()),
      renderer_scheduler_(std::move(scheduler)),
      categorized_worker_pool_(new CategorizedWorkerPool()),
      renderer_binding_(this),
      client_id_(1),
      compositing_mode_watcher_binding_(this) {
  Init(resource_task_queue);
}
