void ChromeContentRendererClient::PrefetchHostName(const char* hostname,
                                                   size_t length) {
  net_predictor_->Resolve(hostname, length);
}
