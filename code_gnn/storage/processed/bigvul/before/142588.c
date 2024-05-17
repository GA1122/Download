void WebstoreAPI::OnInlineInstallFinished(int routing_id,
                                          const std::string& extension_id) {
  RemoveListeners(routing_id, extension_id, &download_progress_listeners_);
  RemoveListeners(routing_id, extension_id, &install_stage_listeners_);
}
