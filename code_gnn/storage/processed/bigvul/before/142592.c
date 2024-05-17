void WebstoreAPI::SendInstallMessageIfObserved(
    const std::string& extension_id,
    api::webstore::InstallStage install_stage) {
  for (ObservedInstallInfoList::const_iterator iter =
           install_stage_listeners_.begin();
       iter != install_stage_listeners_.end();
       ++iter) {
    if (iter->extension_id == extension_id) {
      iter->ipc_sender->Send(new ExtensionMsg_InlineInstallStageChanged(
          iter->routing_id, install_stage));
    }
  }
}
