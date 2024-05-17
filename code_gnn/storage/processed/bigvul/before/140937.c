const OriginAccessEntry& Document::AccessEntryFromURL() {
  if (!access_entry_from_url_) {
    access_entry_from_url_ = std::make_unique<OriginAccessEntry>(
        Url().Protocol(), Url().Host(),
        network::mojom::CorsOriginAccessMatchMode::kAllowRegisterableDomains);
  }
  return *access_entry_from_url_;
}
