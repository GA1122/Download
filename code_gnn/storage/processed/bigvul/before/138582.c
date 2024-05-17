void WriteFromUrlOperation::StartImpl() {
  DCHECK_CURRENTLY_ON(BrowserThread::FILE);

  GetDownloadTarget(base::Bind(
      &WriteFromUrlOperation::Download,
      this,
      base::Bind(
          &WriteFromUrlOperation::VerifyDownload,
          this,
          base::Bind(
              &WriteFromUrlOperation::Unzip,
              this,
              base::Bind(&WriteFromUrlOperation::Write,
                         this,
                         base::Bind(&WriteFromUrlOperation::VerifyWrite,
                                    this,
                                    base::Bind(&WriteFromUrlOperation::Finish,
                                               this)))))));
}
