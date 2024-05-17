void UkmPageLoadMetricsObserver::OnLoadedResource(
    const page_load_metrics::ExtraRequestCompleteInfo&
        extra_request_complete_info) {
  if (was_hidden_)
    return;
  if (extra_request_complete_info.was_cached) {
    cache_bytes_ += extra_request_complete_info.raw_body_bytes;
  } else {
    network_bytes_ += extra_request_complete_info.raw_body_bytes;
  }

  if (extra_request_complete_info.resource_type ==
      content::RESOURCE_TYPE_MAIN_FRAME) {
    DCHECK(!main_frame_timing_.has_value());
    main_frame_timing_ = *extra_request_complete_info.load_timing_info;
  }
}
