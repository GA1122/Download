void RecordWarmupURLFetchAttemptEvent(
    WarmupURLFetchAttemptEvent warmup_url_fetch_event) {
  DCHECK_GT(WarmupURLFetchAttemptEvent::kCount, warmup_url_fetch_event);
  UMA_HISTOGRAM_ENUMERATION("DataReductionProxy.WarmupURL.FetchAttemptEvent",
                            warmup_url_fetch_event,
                            WarmupURLFetchAttemptEvent::kCount);
}
