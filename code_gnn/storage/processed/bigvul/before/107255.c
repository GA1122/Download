void AutomationProvider::OnInitialLoadsComplete() {
  initial_loads_complete_ = true;
  if (is_connected_)
    Send(new AutomationMsg_InitialLoadsComplete());
}
