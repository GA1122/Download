DispatchResponse NetworkHandler::SetRequestInterception(
    std::unique_ptr<protocol::Array<protocol::Network::RequestPattern>>
        patterns) {
  WebContents* web_contents = WebContents::FromRenderFrameHost(host_);
  if (!web_contents)
    return Response::InternalError();

  DevToolsInterceptorController* interceptor =
      DevToolsInterceptorController::FromBrowserContext(
          web_contents->GetBrowserContext());
  if (!interceptor)
    return Response::Error("Interception not supported");

  if (!patterns->length()) {
    interception_handle_.reset();
    return Response::OK();
  }

  std::vector<DevToolsURLRequestInterceptor::Pattern> interceptor_patterns;
  for (size_t i = 0; i < patterns->length(); ++i) {
    base::flat_set<ResourceType> resource_types;
    std::string resource_type = patterns->get(i)->GetResourceType("");
    if (!resource_type.empty()) {
      if (!AddInterceptedResourceType(resource_type, &resource_types)) {
        return Response::InvalidParams(base::StringPrintf(
            "Cannot intercept resources of type '%s'", resource_type.c_str()));
      }
    }
    interceptor_patterns.push_back(DevToolsURLRequestInterceptor::Pattern(
        patterns->get(i)->GetUrlPattern("*"), std::move(resource_types),
        ToInterceptorStage(patterns->get(i)->GetInterceptionStage(
            protocol::Network::InterceptionStageEnum::Request))));
  }

  if (interception_handle_) {
    interception_handle_->UpdatePatterns(std::move(interceptor_patterns));
  } else {
    interception_handle_ = interceptor->StartInterceptingRequests(
        host_->frame_tree_node(), std::move(interceptor_patterns),
        base::Bind(&NetworkHandler::RequestIntercepted,
                   weak_factory_.GetWeakPtr()));
  }

  return Response::OK();
}
