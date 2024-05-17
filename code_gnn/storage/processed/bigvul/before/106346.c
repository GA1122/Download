void SyncBackendHost::Core::ProcessMessage(
    const std::string& name, const JsArgList& args,
    const JsEventHandler* sender) {
  DCHECK_EQ(MessageLoop::current(), host_->frontend_loop_);
  MessageLoop* core_message_loop = host_->core_thread_.message_loop();
  CHECK(core_message_loop);
  core_message_loop->PostTask(
      FROM_HERE,
      NewRunnableMethod(this,
                        &SyncBackendHost::Core::DoProcessMessage,
                        name, args, sender));
}
