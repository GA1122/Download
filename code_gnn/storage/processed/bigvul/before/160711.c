void RenderFrameImpl::ReportPeakMemoryStats() {
  if (!base::FeatureList::IsEnabled(features::kReportRendererPeakMemoryStats))
    return;

  RecordSuffixedRendererMemoryMetrics(peak_memory_metrics_, ".PeakDuringLoad");
  if (!IsMainFrame())
    return;
  RecordSuffixedRendererMemoryMetrics(peak_memory_metrics_,
                                      ".MainFrame.PeakDuringLoad");
  if (!IsControlledByServiceWorker())
    return;
  RecordSuffixedRendererMemoryMetrics(
      peak_memory_metrics_, ".ServiceWorkerControlledMainFrame.PeakDuringLoad");
}
