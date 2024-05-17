void NetworkHandler::RequestIntercepted(
    std::unique_ptr<InterceptedRequestInfo> info) {
  protocol::Maybe<protocol::Network::ErrorReason> error_reason;
  if (info->response_error_code < 0)
    error_reason = NetErrorToString(info->response_error_code);
  frontend_->RequestIntercepted(
      info->interception_id, std::move(info->network_request),
      info->frame_id.ToString(), ResourceTypeToString(info->resource_type),
      info->is_navigation, std::move(info->redirect_url),
      std::move(info->auth_challenge), std::move(error_reason),
      std::move(info->http_response_status_code),
      std::move(info->response_headers));
}
