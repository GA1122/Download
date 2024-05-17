void NavigationRequest::IgnorePipeDisconnection() {
  return request_navigation_client_.set_connection_error_handler(
      base::DoNothing());
}
