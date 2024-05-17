void OomInterventionImpl::StartDetection(
    mojom::blink::OomInterventionHostPtr host,
    mojom::blink::DetectionArgsPtr detection_args,
    bool renderer_pause_enabled,
    bool navigate_ads_enabled) {
  host_ = std::move(host);

  if (CrashMemoryMetricsReporterImpl::Instance().ResetFileDiscriptors())
    return;

  detection_args_ = std::move(detection_args);
  renderer_pause_enabled_ = renderer_pause_enabled;
  navigate_ads_enabled_ = navigate_ads_enabled;

  timer_.Start(TimeDelta(), TimeDelta::FromSeconds(1), FROM_HERE);
}
