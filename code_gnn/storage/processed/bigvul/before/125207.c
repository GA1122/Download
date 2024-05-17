void RenderMessageFilter::OnGetCPUUsage(int* cpu_usage) {
  base::TimeTicks now = base::TimeTicks::Now();
  int64 since_last_sample_ms = (now - cpu_usage_sample_time_).InMilliseconds();
  if (since_last_sample_ms > kCPUUsageSampleIntervalMs) {
    cpu_usage_sample_time_ = now;
    cpu_usage_ = static_cast<int>(process_metrics_->GetCPUUsage());
  }
  *cpu_usage = cpu_usage_;
}
