void Browser::OnDevToolsDisabledChanged() {
  if (profile_->GetPrefs()->GetBoolean(prefs::kDevToolsDisabled))
    content::DevToolsAgentHost::DetachAllClients();
}
