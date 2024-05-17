void WorkerFetchContext::PrepareRequest(ResourceRequest& request,
                                        RedirectType) {
  String user_agent = global_scope_->UserAgent();
  probe::applyUserAgentOverride(global_scope_, &user_agent);
  DCHECK(!user_agent.IsNull());
  request.SetHTTPUserAgent(AtomicString(user_agent));

  request.OverrideLoadingIPCType(WebURLRequest::LoadingIPCType::kMojo);
  WrappedResourceRequest webreq(request);
  web_context_->WillSendRequest(webreq);
}
