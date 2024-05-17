DownloadResourceHandler::~DownloadResourceHandler() {
  if (tab_info_) {
    base::PostTaskWithTraits(
        FROM_HERE, {BrowserThread::UI},
        base::BindOnce(&DeleteOnUIThread, std::move(tab_info_)));
  }
}
