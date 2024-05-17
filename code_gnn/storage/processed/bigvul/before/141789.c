bool ChromeMetricsServiceClient::IsMetricsReportingForceEnabled() {
  return base::CommandLine::ForCurrentProcess()->HasSwitch(
      metrics::switches::kForceEnableMetricsReporting);
}
