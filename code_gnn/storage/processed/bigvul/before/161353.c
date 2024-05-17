bool NetworkHandler::ShouldCancelNavigation(
    const GlobalRequestID& global_request_id) {
  WebContents* web_contents = WebContents::FromRenderFrameHost(host_);
  if (!web_contents)
    return false;
  DevToolsInterceptorController* interceptor =
      DevToolsInterceptorController::FromBrowserContext(
          web_contents->GetBrowserContext());
  return interceptor && interceptor->ShouldCancelNavigation(global_request_id);
}
