void SafeBrowsingPrivateEventRouter::OnDangerousDownloadOpened(
    const GURL& url,
    const std::string& file_name,
    const std::string& download_digest_sha256) {
  api::safe_browsing_private::DangerousDownloadInfo params;
  params.url = url.spec();
  params.file_name = file_name;
  params.download_digest_sha256 = download_digest_sha256;
  params.user_name = GetProfileUserName();

  if (event_router_) {
    auto event_value = std::make_unique<base::ListValue>();
    event_value->Append(params.ToValue());

    auto extension_event = std::make_unique<Event>(
        events::SAFE_BROWSING_PRIVATE_ON_DANGEROUS_DOWNLOAD_OPENED,
        api::safe_browsing_private::OnDangerousDownloadOpened::kEventName,
        std::move(event_value));
    event_router_->BroadcastEvent(std::move(extension_event));
  }

  if (client_) {
    base::Value event(base::Value::Type::DICTIONARY);
    event.SetStringKey(kKeyUrl, params.url);
    event.SetStringKey(kKeyFileName, params.file_name);
    event.SetStringKey(kKeyDownloadDigestSha256, params.download_digest_sha256);
    event.SetStringKey(kKeyProfileUserName, params.user_name);
    ReportRealtimeEvent(kKeyDangerousDownloadEvent, std::move(event));
  }
}
