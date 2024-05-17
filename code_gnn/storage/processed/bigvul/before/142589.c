void WebstoreAPI::OnInlineInstallStart(int routing_id,
                                       IPC::Sender* ipc_sender,
                                       const std::string& extension_id,
                                       int listeners_mask) {
  if (listeners_mask & api::webstore::INSTALL_STAGE_LISTENER) {
    install_stage_listeners_.push_back(
        ObservedInstallInfo(routing_id, extension_id, ipc_sender));
  }

  if (listeners_mask & api::webstore::DOWNLOAD_PROGRESS_LISTENER) {
    download_progress_listeners_.push_back(
        ObservedInstallInfo(routing_id, extension_id, ipc_sender));
  }
}
