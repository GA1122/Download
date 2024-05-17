DevToolsDownloadManagerDelegate::DevToolsDownloadManagerDelegate()
    : download_manager_(nullptr), proxy_download_delegate_(nullptr) {
  g_devtools_manager_delegate = this;
}
