void PageHandler::NavigationReset(NavigationRequest* navigation_request) {
  if (!navigate_callback_)
    return;
  WebContentsImpl* web_contents = GetWebContents();
  if (!web_contents) {
    navigate_callback_->sendFailure(Response::InternalError());
    return;
  }

  std::string frame_id =
      web_contents->GetMainFrame()->GetDevToolsFrameToken().ToString();
  bool success = navigation_request->net_error() != net::OK;
  std::string error_string =
      net::ErrorToString(navigation_request->net_error());
  navigate_callback_->sendSuccess(
      frame_id,
      Maybe<std::string>(
          navigation_request->devtools_navigation_token().ToString()),
      success ? Maybe<std::string>(error_string) : Maybe<std::string>());
  navigate_callback_.reset();
}
