void RenderThreadImpl::SetProcessBackgrounded(bool backgrounded) {
  base::TimerSlack timer_slack = base::TIMER_SLACK_NONE;
  if (backgrounded)
    timer_slack = base::TIMER_SLACK_MAXIMUM;
  base::MessageLoop::current()->SetTimerSlack(timer_slack);

  renderer_scheduler_->SetRendererBackgrounded(backgrounded);
  if (backgrounded) {
    needs_to_record_first_active_paint_ = false;
    GetRendererScheduler()->DefaultTaskRunner()->PostDelayedTask(
        FROM_HERE,
        base::BindOnce(&RenderThreadImpl::RecordMemoryUsageAfterBackgrounded,
                       base::Unretained(this), "5min",
                       process_foregrounded_count_),
        base::TimeDelta::FromMinutes(5));
    GetRendererScheduler()->DefaultTaskRunner()->PostDelayedTask(
        FROM_HERE,
        base::BindOnce(&RenderThreadImpl::RecordMemoryUsageAfterBackgrounded,
                       base::Unretained(this), "10min",
                       process_foregrounded_count_),
        base::TimeDelta::FromMinutes(10));
    GetRendererScheduler()->DefaultTaskRunner()->PostDelayedTask(
        FROM_HERE,
        base::BindOnce(&RenderThreadImpl::RecordMemoryUsageAfterBackgrounded,
                       base::Unretained(this), "15min",
                       process_foregrounded_count_),
        base::TimeDelta::FromMinutes(15));
    was_backgrounded_time_ = base::TimeTicks::Now();
  } else {
    process_foregrounded_count_++;
  }
}
