void ChromeRenderMessageFilter::OpenChannelToTabOnUIThread(
    int source_process_id, int source_routing_id,
    int receiver_port_id,
    int tab_id,
    const std::string& extension_id,
    const std::string& channel_name) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  extensions::ExtensionSystem::Get(profile_)->message_service()->
      OpenChannelToTab(
          source_process_id, source_routing_id, receiver_port_id,
          tab_id, extension_id, channel_name);
}
