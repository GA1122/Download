void RenderThreadImpl::InitializeCompositorThread() {
  base::Thread::Options options;
#if defined(OS_ANDROID)
  options.priority = base::ThreadPriority::DISPLAY;
#endif
  compositor_thread_ =
      blink::scheduler::WebThreadBase::CreateCompositorThread(options);
  blink_platform_impl_->SetCompositorThread(compositor_thread_.get());
  compositor_task_runner_ = compositor_thread_->GetTaskRunner();
  compositor_task_runner_->PostTask(
      FROM_HERE,
      base::BindOnce(base::IgnoreResult(&ThreadRestrictions::SetIOAllowed),
                     false));
#if defined(OS_LINUX)
  ChildThreadImpl::current()->SetThreadPriority(compositor_thread_->ThreadId(),
                                                base::ThreadPriority::DISPLAY);
#endif

  if (!base::FeatureList::IsEnabled(features::kMojoInputMessages)) {
    SynchronousInputHandlerProxyClient* synchronous_input_handler_proxy_client =
        nullptr;
#if defined(OS_ANDROID)
    if (GetContentClient()->UsingSynchronousCompositing()) {
      sync_compositor_message_filter_ =
          new SynchronousCompositorFilter(compositor_task_runner_);
      AddFilter(sync_compositor_message_filter_.get());
      synchronous_input_handler_proxy_client =
          sync_compositor_message_filter_.get();
    }
#endif
    scoped_refptr<InputEventFilter> compositor_input_event_filter(
        new InputEventFilter(main_input_callback_.callback(),
                             main_thread_compositor_task_runner_,
                             compositor_task_runner_));
    InputHandlerManagerClient* input_handler_manager_client =
        compositor_input_event_filter.get();
    input_event_filter_ = compositor_input_event_filter;
    input_handler_manager_.reset(new InputHandlerManager(
        compositor_task_runner_, input_handler_manager_client,
        synchronous_input_handler_proxy_client, renderer_scheduler_.get()));
  } else {
#if defined(OS_ANDROID)
    DCHECK(!GetContentClient()->UsingSynchronousCompositing());
#endif
  }
}
