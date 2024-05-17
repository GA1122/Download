ResourceLoadPriority FrameFetchContext::ModifyPriorityForExperiments(
    ResourceLoadPriority priority) const {
  if (!GetSettings())
    return priority;

  WebEffectiveConnectionType max_effective_connection_type_threshold =
      GetSettings()->GetLowPriorityIframesThreshold();

  if (max_effective_connection_type_threshold <=
      WebEffectiveConnectionType::kTypeOffline) {
    return priority;
  }

  WebEffectiveConnectionType effective_connection_type =
      GetNetworkStateNotifier().EffectiveType();

  if (effective_connection_type <= WebEffectiveConnectionType::kTypeOffline) {
    return priority;
  }

  if (effective_connection_type > max_effective_connection_type_threshold) {
    return priority;
  }

  if (GetFrame()->IsMainFrame()) {
    DEFINE_STATIC_LOCAL(EnumerationHistogram, main_frame_priority_histogram,
                        ("LowPriorityIframes.MainFrameRequestPriority",
                         static_cast<int>(ResourceLoadPriority::kHighest) + 1));
    main_frame_priority_histogram.Count(static_cast<int>(priority));
    return priority;
  }

  DEFINE_STATIC_LOCAL(EnumerationHistogram, iframe_priority_histogram,
                      ("LowPriorityIframes.IframeRequestPriority",
                       static_cast<int>(ResourceLoadPriority::kHighest) + 1));
  iframe_priority_histogram.Count(static_cast<int>(priority));
  if (priority >= ResourceLoadPriority::kHigh)
    return ResourceLoadPriority::kLow;
  return ResourceLoadPriority::kLowest;
}
