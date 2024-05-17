void RenderThreadImpl::ProcessPurgeAndSuspend() {
  if (!RendererIsHidden())
    return;

  if (!base::FeatureList::IsEnabled(features::kPurgeAndSuspend))
    return;

  base::MemoryCoordinatorClientRegistry::GetInstance()->PurgeMemory();
  needs_to_record_first_active_paint_ = true;

  RendererMemoryMetrics memory_metrics;
  if (!GetRendererMemoryMetrics(&memory_metrics))
    return;

  purge_and_suspend_memory_metrics_ = memory_metrics;
  GetRendererScheduler()->DefaultTaskRunner()->PostDelayedTask(
      FROM_HERE,
      base::BindOnce(
          &RenderThreadImpl::RecordPurgeAndSuspendMemoryGrowthMetrics,
          base::Unretained(this), "30min", process_foregrounded_count_),
      base::TimeDelta::FromMinutes(30));
  GetRendererScheduler()->DefaultTaskRunner()->PostDelayedTask(
      FROM_HERE,
      base::BindOnce(
          &RenderThreadImpl::RecordPurgeAndSuspendMemoryGrowthMetrics,
          base::Unretained(this), "60min", process_foregrounded_count_),
      base::TimeDelta::FromMinutes(60));
  GetRendererScheduler()->DefaultTaskRunner()->PostDelayedTask(
      FROM_HERE,
      base::BindOnce(
          &RenderThreadImpl::RecordPurgeAndSuspendMemoryGrowthMetrics,
          base::Unretained(this), "90min", process_foregrounded_count_),
      base::TimeDelta::FromMinutes(90));
}
