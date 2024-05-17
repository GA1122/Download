void BrowserRenderProcessHost::OnExtensionRemoveListener(
    const std::string& event_name) {
  if (profile()->GetExtensionMessageService()) {
    profile()->GetExtensionMessageService()->RemoveEventListener(
        event_name, id());
  }
}
