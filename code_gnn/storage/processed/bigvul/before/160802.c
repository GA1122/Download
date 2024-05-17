void RenderViewImpl::OnForceRedraw(const ui::LatencyInfo& latency_info) {
  if (RenderWidgetCompositor* rwc = compositor()) {
    rwc->QueueSwapPromise(
        std::make_unique<AlwaysDrawSwapPromise>(latency_info));
    rwc->SetNeedsForcedRedraw();
  }
}
