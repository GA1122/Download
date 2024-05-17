void DownloadItemImpl::Start(
    std::unique_ptr<DownloadFile> file,
    std::unique_ptr<DownloadRequestHandleInterface> req_handle,
    const DownloadCreateInfo& new_create_info) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  DCHECK(!download_file_.get());
  DVLOG(20) << __func__ << "() this=" << DebugString(true);
  RecordDownloadCount(START_COUNT);

  download_file_ = std::move(file);
  job_ = DownloadJobFactory::CreateJob(this, std::move(req_handle),
                                       new_create_info, false);
  if (job_->IsParallelizable()) {
    RecordParallelizableDownloadCount(START_COUNT, IsParallelDownloadEnabled());
  }

  deferred_interrupt_reason_ = DOWNLOAD_INTERRUPT_REASON_NONE;

  if (state_ == CANCELLED_INTERNAL) {
    ReleaseDownloadFile(true);
    job_->Cancel(true);
    return;
  }

  DCHECK(state_ == INITIAL_INTERNAL || state_ == RESUMING_INTERNAL);

  DCHECK(state_ != INITIAL_INTERNAL || GetTargetFilePath().empty());

  if (new_create_info.result != DOWNLOAD_INTERRUPT_REASON_NONE) {
    DCHECK(!download_file_.get());

    DCHECK(new_create_info.save_info);

    int64_t offset = new_create_info.save_info->offset;
    std::unique_ptr<crypto::SecureHash> hash_state =
        new_create_info.save_info->hash_state
            ? new_create_info.save_info->hash_state->Clone()
            : nullptr;

    destination_info_.received_bytes = offset;
    hash_state_ = std::move(hash_state);
    destination_info_.hash.clear();
    deferred_interrupt_reason_ = new_create_info.result;
    received_slices_.clear();
    TransitionTo(INTERRUPTED_TARGET_PENDING_INTERNAL);
    DetermineDownloadTarget();
    return;
  }

  if (state_ == INITIAL_INTERNAL) {
    RecordDownloadCount(NEW_DOWNLOAD_COUNT);
    if (job_->IsParallelizable()) {
      RecordParallelizableDownloadCount(NEW_DOWNLOAD_COUNT,
                                        IsParallelDownloadEnabled());
    }
    RecordDownloadMimeType(mime_type_);
    if (!GetBrowserContext()->IsOffTheRecord()) {
      RecordDownloadCount(NEW_DOWNLOAD_COUNT_NORMAL_PROFILE);
      RecordDownloadMimeTypeForNormalProfile(mime_type_);
    }
  }

  DCHECK(download_file_);
  DCHECK(job_);

  if (state_ == RESUMING_INTERNAL)
    UpdateValidatorsOnResumption(new_create_info);

  if (!job_->IsParallelizable() && !received_slices_.empty()) {
    destination_info_.received_bytes =
        GetMaxContiguousDataBlockSizeFromBeginning(received_slices_);
    received_slices_.clear();
  }

  TransitionTo(TARGET_PENDING_INTERNAL);

  job_->Start(download_file_.get(),
              base::Bind(&DownloadItemImpl::OnDownloadFileInitialized,
                         weak_ptr_factory_.GetWeakPtr()),
              GetReceivedSlices());
}
