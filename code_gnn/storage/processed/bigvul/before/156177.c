RendererSchedulerImpl::CreateMaxQueueingTimeMetric() {
  return base::SingleSampleMetricsFactory::Get()->CreateCustomCountsMetric(
      "RendererScheduler.MaxQueueingTime", 1, 10000, 50);
}
