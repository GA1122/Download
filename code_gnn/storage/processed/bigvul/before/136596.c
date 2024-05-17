void DocumentLoader::CancelLoadAfterCSPDenied(
    const ResourceResponse& response) {
  probe::CanceledAfterReceivedResourceResponse(
      frame_, this, MainResourceIdentifier(), response, GetResource());

  SetWasBlockedAfterCSP();

  ClearResource();
  content_security_policy_.Clear();
  KURL blocked_url = SecurityOrigin::UrlWithUniqueSecurityOrigin();
  original_request_.SetURL(blocked_url);
  request_.SetURL(blocked_url);
  redirect_chain_.pop_back();
  AppendRedirect(blocked_url);
  response_ = ResourceResponse(blocked_url, "text/html");
  FinishedLoading(CurrentTimeTicks());

  return;
}
