void SyncBackendHost::Core::RouteJsEventOnFrontendLoop(
    const std::string& name, const JsEventDetails& details) {
  if (!host_ || !parent_router_)
    return;

  DCHECK_EQ(MessageLoop::current(), host_->frontend_loop_);

  parent_router_->RouteJsEvent(name, details);
}
