void WebstoreAPI::OnBeginExtensionDownload(const std::string& extension_id) {
  SendInstallMessageIfObserved(extension_id,
                               api::webstore::INSTALL_STAGE_DOWNLOADING);
}
