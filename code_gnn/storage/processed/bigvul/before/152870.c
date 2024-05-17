void UkmPageLoadMetricsObserver::ReportLayoutStability(
    const page_load_metrics::PageLoadExtraInfo& info) {
  if (!base::FeatureList::IsEnabled(blink::features::kJankTracking))
    return;

  float jank_score = info.main_frame_render_data.layout_jank_score;
  int64_t ukm_value =
      static_cast<int>(roundf(std::min(jank_score, 10.0f) * 100.0f));

  ukm::builders::PageLoad builder(info.source_id);
  builder.SetLayoutStability_JankScore(ukm_value);
  builder.Record(ukm::UkmRecorder::Get());

  int32_t uma_value =
      static_cast<int>(roundf(std::min(jank_score, 10.0f) * 10.0f));
  UMA_HISTOGRAM_COUNTS_100("PageLoad.Experimental.LayoutStability.JankScore",
                           uma_value);
}
