void AutomationProvider::OnChannelError() {
  VLOG(1) << "AutomationProxy went away, shutting down app.";
  AutomationProviderList::GetInstance()->RemoveProvider(this);
}
