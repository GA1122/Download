NavigationRequest::~NavigationRequest() {
  TRACE_EVENT_ASYNC_END0("navigation", "NavigationRequest", this);
  if (state_ == STARTED) {
    devtools_instrumentation::OnNavigationRequestFailed(
        *this, network::URLLoaderCompletionStatus(net::ERR_ABORTED));
  }
  navigation_handle_.reset();
}
