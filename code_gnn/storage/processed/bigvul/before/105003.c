HttpBridge::RequestContextGetter::GetURLRequestContext() {
  if (!context_) {
    net::URLRequestContext* baseline_context =
        baseline_context_getter_->GetURLRequestContext();
    context_ = new RequestContext(baseline_context);
    baseline_context_getter_ = NULL;
  }

  if (is_user_agent_set())
    context_->set_user_agent(user_agent_);

  return context_;
}
