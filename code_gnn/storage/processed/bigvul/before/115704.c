HostNPScriptObject::~HostNPScriptObject() {
  CHECK_EQ(base::PlatformThread::CurrentId(), np_thread_id_);

  if (desktop_environment_.get()) {
    desktop_environment_->Shutdown();
  }

  HostLogHandler::UnregisterLoggingScriptObject(this);

  plugin_message_loop_proxy_->Detach();

  if (nat_policy_.get()) {
    base::WaitableEvent nat_policy_stopped_(true, false);
    nat_policy_->StopWatching(&nat_policy_stopped_);
    nat_policy_stopped_.Wait();
    nat_policy_.reset();
  }

  disconnected_event_.Reset();
  DisconnectInternal();
  disconnected_event_.Wait();

  host_context_.Stop();
}
