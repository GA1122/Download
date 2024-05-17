void DevToolsDownloadManagerDelegate::Shutdown() {
  if (proxy_download_delegate_)
    proxy_download_delegate_->Shutdown();
  download_manager_ = nullptr;
}
