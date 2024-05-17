void InitializeOnDBSequence(
    ResourcePrefetchPredictor::RedirectDataMap* host_redirect_data,
    ResourcePrefetchPredictor::OriginDataMap* origin_data) {
  host_redirect_data->InitializeOnDBSequence();
  origin_data->InitializeOnDBSequence();
}
