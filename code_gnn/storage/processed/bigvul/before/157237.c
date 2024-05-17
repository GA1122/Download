bool WebMediaPlayerImpl::IsSuspendedForTesting() {
  return pipeline_controller_.IsPipelineSuspended();
}
