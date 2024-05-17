void SyncBackendHost::Core::RouteJsMessageReplyOnFrontendLoop(
    const std::string& name, const JsArgList& args,
    const JsEventHandler* target) {
  if (!host_ || !parent_router_)
    return;

  DCHECK_EQ(MessageLoop::current(), host_->frontend_loop_);

  parent_router_->RouteJsMessageReply(name, args, target);
}
