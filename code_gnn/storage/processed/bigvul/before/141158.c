void Document::MaybeHandleHttpRefresh(const String& content,
                                      HttpRefreshType http_refresh_type) {
  if (is_view_source_ || !frame_)
    return;

  double delay;
  String refresh_url_string;
  if (!ParseHTTPRefresh(content,
                        http_refresh_type == kHttpRefreshFromMetaTag
                            ? IsHTMLSpace<UChar>
                            : nullptr,
                        delay, refresh_url_string))
    return;
  KURL refresh_url =
      refresh_url_string.IsEmpty() ? Url() : CompleteURL(refresh_url_string);

  if (refresh_url.ProtocolIsJavaScript()) {
    String message =
        "Refused to refresh " + url_.ElidedString() + " to a javascript: URL";
    AddConsoleMessage(ConsoleMessage::Create(
        kSecurityMessageSource, mojom::ConsoleMessageLevel::kError, message));
    return;
  }

  if (http_refresh_type == kHttpRefreshFromMetaTag &&
      IsSandboxed(kSandboxAutomaticFeatures)) {
    String message =
        "Refused to execute the redirect specified via '<meta "
        "http-equiv='refresh' content='...'>'. The document is sandboxed, and "
        "the 'allow-scripts' keyword is not set.";
    AddConsoleMessage(ConsoleMessage::Create(
        kSecurityMessageSource, mojom::ConsoleMessageLevel::kError, message));
    return;
  }
  if (http_refresh_type == kHttpRefreshFromHeader) {
    UseCounter::Count(this, WebFeature::kRefreshHeader);
  }
  frame_->GetNavigationScheduler().ScheduleRedirect(delay, refresh_url,
                                                    http_refresh_type);
}
