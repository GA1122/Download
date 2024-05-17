  void CanDownloadFor(WebContents* web_contents) {
    download_request_limiter_->CanDownloadImpl(
        web_contents,
        "GET",   
        base::Bind(&DownloadRequestLimiterTest::ContinueDownload,
                   base::Unretained(this)));
    base::RunLoop().RunUntilIdle();
  }
