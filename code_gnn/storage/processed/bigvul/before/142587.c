void WebstoreAPI::OnDownloadProgress(const std::string& extension_id,
                                     int percent_downloaded) {
  for (ObservedInstallInfoList::const_iterator iter =
           download_progress_listeners_.begin();
       iter != download_progress_listeners_.end();
       ++iter) {
    if (iter->extension_id == extension_id) {
      iter->ipc_sender->Send(new ExtensionMsg_InlineInstallDownloadProgress(
          iter->routing_id, percent_downloaded));
    }
  }
}
