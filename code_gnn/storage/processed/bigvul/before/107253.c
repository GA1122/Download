void AutomationProvider::OnChannelConnected(int pid) {
  is_connected_ = true;
  LOG(INFO) << "Testing channel connected, sending hello message";

  channel_->Send(new AutomationMsg_Hello(GetProtocolVersion()));
  if (initial_loads_complete_)
    Send(new AutomationMsg_InitialLoadsComplete());
}
