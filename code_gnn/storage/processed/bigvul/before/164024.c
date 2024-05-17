void DownloadManagerImpl::DownloadInterrupted(
    download::DownloadItemImpl* download) {
  WebContents* web_contents = DownloadItemUtils::GetWebContents(download);
  if (!web_contents) {
    download::RecordDownloadCountWithSource(
        download::INTERRUPTED_WITHOUT_WEBCONTENTS, download->download_source());
  }
}
