void RenderThreadImpl::OnPurgeMemory() {
  RendererMemoryMetrics metrics;
  if (!GetRendererMemoryMetrics(&metrics))
    return;

  GetRendererScheduler()->DefaultTaskRunner()->PostDelayedTask(
      FROM_HERE,
      base::BindOnce(&RenderThreadImpl::RecordPurgeMemory,
                     base::Unretained(this), std::move(metrics)),
      base::TimeDelta::FromSeconds(2));

  OnTrimMemoryImmediately();
  ReleaseFreeMemory();
  ClearMemory();
  if (blink_platform_impl_)
    blink::WebMemoryCoordinator::OnPurgeMemory();
}
