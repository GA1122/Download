DownloadItemImpl::DownloadItemImpl(
    DownloadItemImplDelegate* delegate,
    uint32_t download_id,
    const base::FilePath& path,
    const GURL& url,
    const std::string& mime_type,
    std::unique_ptr<DownloadRequestHandleInterface> request_handle)
    : request_info_(url),
      guid_(base::GenerateGUID()),
      download_id_(download_id),
      mime_type_(mime_type),
      original_mime_type_(mime_type),
      start_tick_(base::TimeTicks::Now()),
      state_(IN_PROGRESS_INTERNAL),
      delegate_(delegate),
      destination_info_(path, path, 0, false, std::string(), base::Time()),
      is_updating_observers_(false),
      weak_ptr_factory_(this) {
  job_ = DownloadJobFactory::CreateJob(this, std::move(request_handle),
                                       DownloadCreateInfo(), true);
  delegate_->Attach();
  Init(true  , TYPE_SAVE_PAGE_AS);
}
