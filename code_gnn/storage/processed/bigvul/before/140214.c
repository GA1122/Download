void WebResourceService::StartAfterDelay() {
  if (resource_request_allowed_notifier_.ResourceRequestsAllowed())
    OnResourceRequestsAllowed();
}
