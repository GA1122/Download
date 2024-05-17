FetchContext::FetchContext() : platform_probe_sink_(new PlatformProbeSink) {
  platform_probe_sink_->addPlatformTraceEventsAgent(
      new PlatformTraceEventsAgent);
}
