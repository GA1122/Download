void ChromeRenderMessageFilter::OpenChannelToExtensionOnUIThread(
    int source_process_id, int source_routing_id,
    int receiver_port_id,
    const std::string& source_extension_id,
    const std::string& target_extension_id,
    const std::string& channel_name) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  extensions::ExtensionSystem::Get(profile_)->message_service()->
      OpenChannelToExtension(
          source_process_id, source_routing_id, receiver_port_id,
          source_extension_id, target_extension_id, channel_name);
}
