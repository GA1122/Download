void DownloadItemImpl::Completed() {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);

  DVLOG(20) << __func__ << "() " << DebugString(false);

  DCHECK(AllDataSaved());
  destination_info_.end_time = base::Time::Now();
  TransitionTo(COMPLETE_INTERNAL);

  bool is_parallelizable = job_ && job_->IsParallelizable();
  RecordDownloadCompleted(start_tick_, GetReceivedBytes(), is_parallelizable);
  if (!GetBrowserContext()->IsOffTheRecord()) {
    RecordDownloadCount(COMPLETED_COUNT_NORMAL_PROFILE);
  }
  if (is_parallelizable) {
    RecordParallelizableDownloadCount(COMPLETED_COUNT,
                                      IsParallelDownloadEnabled());
    int64_t content_length = -1;
    if (response_headers_->response_code() != net::HTTP_PARTIAL_CONTENT) {
      content_length = response_headers_->GetContentLength();
    } else {
      int64_t first_byte = -1;
      int64_t last_byte = -1;
      response_headers_->GetContentRangeFor206(&first_byte, &last_byte,
                                               &content_length);
    }
    if (content_length > 0)
      RecordParallelizableContentLength(content_length);
  }

  if (auto_opened_) {
  } else if (GetOpenWhenComplete() ||
             ShouldOpenFileBasedOnExtension() ||
             IsTemporary()) {
    if (!IsTemporary())
      OpenDownload();

    auto_opened_ = true;
  }
  UpdateObservers();
}
