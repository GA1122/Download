void SyncBackendHost::Core::DoProcessMessage(
    const std::string& name, const JsArgList& args,
    const JsEventHandler* sender) {
  DCHECK_EQ(MessageLoop::current(), host_->core_thread_.message_loop());
  syncapi_->GetJsBackend()->ProcessMessage(name, args, sender);
}
