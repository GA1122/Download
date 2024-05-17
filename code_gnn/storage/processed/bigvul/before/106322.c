const JsEventRouter* SyncBackendHost::Core::GetParentJsEventRouter() const {
  DCHECK_EQ(MessageLoop::current(), host_->frontend_loop_);
  return parent_router_;
}
