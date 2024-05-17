  void StartExtractZipFeatures() {
    DCHECK_CURRENTLY_ON(BrowserThread::UI);
    DCHECK(item_);   
    zip_analysis_start_time_ = base::TimeTicks::Now();
    analyzer_ = new SandboxedZipAnalyzer(
        item_->GetFullPath(),
        base::Bind(&CheckClientDownloadRequest::OnZipAnalysisFinished,
                   weakptr_factory_.GetWeakPtr()));
    analyzer_->Start();
  }
