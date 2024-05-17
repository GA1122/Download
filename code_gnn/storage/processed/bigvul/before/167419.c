DevToolsDownloadManagerDelegate::~DevToolsDownloadManagerDelegate() {
  DevToolsDownloadManagerDelegate* download_delegate = GetInstance();
  download_delegate->download_manager_->SetDelegate(
      download_delegate->proxy_download_delegate_);
  download_delegate->download_manager_ = nullptr;

  if (download_manager_) {
    download_manager_->SetDelegate(proxy_download_delegate_);
    download_manager_ = nullptr;
  }
  g_devtools_manager_delegate = nullptr;
}
