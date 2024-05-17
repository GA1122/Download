int ChromeNetworkDelegate::OnBeforeURLRequest(
    net::URLRequest* request,
    const net::CompletionCallback& callback,
    GURL* new_url) {
#if defined(ENABLE_CONFIGURATION_POLICY)
  if (url_blacklist_manager_ &&
      url_blacklist_manager_->IsRequestBlocked(*request)) {
    request->net_log().AddEvent(
        net::NetLog::TYPE_CHROME_POLICY_ABORTED_REQUEST,
        net::NetLog::StringCallback("url",
                                    &request->url().possibly_invalid_spec()));
    return net::ERR_BLOCKED_BY_ADMINISTRATOR;
  }
#endif

  ForwardRequestStatus(REQUEST_STARTED, request, profile_);

  if (!enable_referrers_->GetValue())
    request->SetReferrer(std::string());
  if (enable_do_not_track_ && enable_do_not_track_->GetValue())
    request->SetExtraRequestHeaderByName(kDNTHeader, "1", true  );

  if (client_hints_) {
    request->SetExtraRequestHeaderByName(
        ClientHints::kDevicePixelRatioHeader,
        client_hints_->GetDevicePixelRatioHeader(), true);
  }

  bool force_safe_search = force_google_safe_search_ &&
                           force_google_safe_search_->GetValue();

  net::CompletionCallback wrapped_callback = callback;
  if (force_safe_search) {
    wrapped_callback = base::Bind(&ForceGoogleSafeSearchCallbackWrapper,
                                  callback,
                                  base::Unretained(request),
                                  base::Unretained(new_url));
  }

  int rv = ExtensionWebRequestEventRouter::GetInstance()->OnBeforeRequest(
      profile_, extension_info_map_.get(), request, wrapped_callback,
      new_url);

  if (force_safe_search && rv == net::OK && new_url->is_empty())
    ForceGoogleSafeSearch(request, new_url);

  if (connect_interceptor_)
    connect_interceptor_->WitnessURLRequest(request);

  return rv;
}
