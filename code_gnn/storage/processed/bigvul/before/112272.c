void ChromeRenderMessageFilter::OnExtensionCloseChannel(int port_id,
                                                        bool connection_error) {
  if (!content::RenderProcessHost::FromID(render_process_id_))
    return;   

  ExtensionMessageService* message_service =
      ExtensionSystem::Get(profile_)->message_service();
  if (message_service)
    message_service->CloseChannel(port_id, connection_error);
}
