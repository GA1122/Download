void ChromeNetworkDelegate::OnResponseStarted(net::URLRequest* request) {
  TRACE_EVENT_ASYNC_STEP_PAST0("net", "URLRequest", request, "ResponseStarted");
  ExtensionWebRequestEventRouter::GetInstance()->OnResponseStarted(
      profile_, extension_info_map_.get(), request);
  ForwardProxyErrors(request, event_router_.get(), profile_);
}
