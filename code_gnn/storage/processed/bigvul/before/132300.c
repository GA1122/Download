void RenderFrameImpl::didClearWindowObject(blink::WebLocalFrame* frame) {
  DCHECK(!frame_ || frame_ == frame);

  int enabled_bindings = render_view_->GetEnabledBindings();

  if (enabled_bindings & BINDINGS_POLICY_WEB_UI)
    WebUIExtension::Install(frame);

  if (enabled_bindings & BINDINGS_POLICY_DOM_AUTOMATION)
    DomAutomationController::Install(this, frame);

  if (enabled_bindings & BINDINGS_POLICY_STATS_COLLECTION)
    StatsCollectionController::Install(frame);

  const base::CommandLine& command_line =
      *base::CommandLine::ForCurrentProcess();

  if (command_line.HasSwitch(cc::switches::kEnableGpuBenchmarking))
    GpuBenchmarking::Install(frame);

  if (command_line.HasSwitch(switches::kEnableMemoryBenchmarking))
    MemoryBenchmarkingExtension::Install(frame);

  if (command_line.HasSwitch(switches::kEnableSkiaBenchmarking))
    SkiaBenchmarking::Install(frame);

  FOR_EACH_OBSERVER(RenderViewObserver, render_view_->observers(),
                    DidClearWindowObject(frame));
  FOR_EACH_OBSERVER(RenderFrameObserver, observers_, DidClearWindowObject());
}
