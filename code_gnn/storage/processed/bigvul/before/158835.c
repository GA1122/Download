void Browser::OnDevToolsAvailabilityChanged() {
  using DTPH = policy::DeveloperToolsPolicyHandler;
  if (DTPH::GetDevToolsAvailability(profile_->GetPrefs()) !=
      DTPH::Availability::kAllowed) {
    content::DevToolsAgentHost::DetachAllClients();
  }
}
