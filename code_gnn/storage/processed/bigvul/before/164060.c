void DownloadManagerImpl::ResumeInterruptedDownload(
    std::unique_ptr<download::DownloadUrlParameters> params,
    const GURL& site_url) {
  BeginDownloadInternal(std::move(params), nullptr  ,
                        nullptr  , false, site_url);
}
