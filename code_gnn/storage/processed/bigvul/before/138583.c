void WriteFromUrlOperation::VerifyDownload(const base::Closure& continuation) {
  DCHECK_CURRENTLY_ON(BrowserThread::FILE);

  if (IsCancelled()) {
    return;
  }

  if (hash_.empty()) {
    BrowserThread::PostTask(BrowserThread::FILE, FROM_HERE, continuation);
    return;
  }

  SetStage(image_writer_api::STAGE_VERIFYDOWNLOAD);

  GetMD5SumOfFile(
      image_path_,
      0,
      0,
      kProgressComplete,
      base::Bind(
          &WriteFromUrlOperation::VerifyDownloadCompare, this, continuation));
}
