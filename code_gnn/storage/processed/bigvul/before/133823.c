  void StartExtractFileFeatures() {
    DCHECK_CURRENTLY_ON(BrowserThread::UI);
    DCHECK(item_);   
    BrowserThread::GetBlockingPool()->PostWorkerTaskWithShutdownBehavior(
        FROM_HERE,
        base::Bind(&CheckClientDownloadRequest::ExtractFileFeatures,
                   this, item_->GetFullPath()),
        base::SequencedWorkerPool::CONTINUE_ON_SHUTDOWN);
  }
