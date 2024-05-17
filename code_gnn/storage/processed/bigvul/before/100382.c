void BrowserRenderProcessHost::OnExtensionAddListener(
    const std::string& event_name) {
  if (profile()->GetExtensionMessageService()) {
    profile()->GetExtensionMessageService()->AddEventListener(
        event_name, id());
  }
}
