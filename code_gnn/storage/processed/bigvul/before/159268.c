void DownloadManagerImpl::ResumeInterruptedDownload(
    std::unique_ptr<content::DownloadUrlParameters> params,
    uint32_t id) {
  BeginDownloadInternal(std::move(params), id);
}
