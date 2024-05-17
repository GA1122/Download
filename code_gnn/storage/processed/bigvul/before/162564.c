void Resource::ResponseReceived(const ResourceResponse& response,
                                std::unique_ptr<WebDataConsumerHandle>) {
  response_timestamp_ = CurrentTime();
  if (preload_discovery_time_) {
    int time_since_discovery = static_cast<int>(
        1000 * (CurrentTimeTicksInSeconds() - preload_discovery_time_));
    DEFINE_STATIC_LOCAL(CustomCountHistogram,
                        preload_discovery_to_first_byte_histogram,
                        ("PreloadScanner.TTFB", 0, 10000, 50));
    preload_discovery_to_first_byte_histogram.Count(time_since_discovery);
  }

  if (is_revalidating_) {
    if (response.HttpStatusCode() == 304) {
      RevalidationSucceeded(response);
      return;
    }
    RevalidationFailed();
  }
  SetResponse(response);
  String encoding = response.TextEncodingName();
  if (!encoding.IsNull())
    SetEncoding(encoding);
}
