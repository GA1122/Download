bool AutomationProvider::InitializeChannel(const std::string& channel_id) {
  TRACE_EVENT_BEGIN("AutomationProvider::InitializeChannel", 0, "");

  std::string effective_channel_id = channel_id;

  bool use_named_interface =
      channel_id.find(automation::kNamedInterfacePrefix) == 0;
  if (use_named_interface) {
    effective_channel_id = channel_id.substr(
        strlen(automation::kNamedInterfacePrefix));
    if (effective_channel_id.length() <= 0)
      return false;
  }

  if (!automation_resource_message_filter_.get()) {
    automation_resource_message_filter_ = new AutomationResourceMessageFilter;
  }

  channel_.reset(new IPC::SyncChannel(
      effective_channel_id,
      use_named_interface ? IPC::Channel::MODE_NAMED_SERVER
                          : IPC::Channel::MODE_CLIENT,
      this,
      g_browser_process->io_thread()->message_loop(),
      true, g_browser_process->shutdown_event()));
  channel_->AddFilter(automation_resource_message_filter_);

  TRACE_EVENT_END("AutomationProvider::InitializeChannel", 0, "");

  return true;
}
