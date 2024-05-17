void BrowserRenderProcessHost::OnExtensionCloseChannel(int port_id) {
  if (profile()->GetExtensionMessageService()) {
    profile()->GetExtensionMessageService()->CloseChannel(port_id);
  }
}
