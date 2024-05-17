RenderThreadImpl::RenderThreadImpl(
    std::unique_ptr<base::MessageLoop> main_message_loop,
    std::unique_ptr<blink::scheduler::RendererScheduler> scheduler)
    : ChildThreadImpl(Options::Builder()
                          .AutoStartServiceManagerConnection(false)
                          .ConnectToBrowser(true)
                          .Build()),
      renderer_scheduler_(std::move(scheduler)),
      main_message_loop_(std::move(main_message_loop)),
      categorized_worker_pool_(new CategorizedWorkerPool()),
      is_scroll_animator_enabled_(false),
      renderer_binding_(this),
      compositing_mode_watcher_binding_(this) {
  scoped_refptr<base::SingleThreadTaskRunner> test_task_counter;
  DCHECK(base::CommandLine::ForCurrentProcess()->HasSwitch(
      switches::kRendererClientId));
  base::StringToInt(base::CommandLine::ForCurrentProcess()->GetSwitchValueASCII(
                        switches::kRendererClientId),
                    &client_id_);
  Init(test_task_counter);
}
