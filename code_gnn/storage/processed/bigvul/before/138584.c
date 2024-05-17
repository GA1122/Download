void WriteFromUrlOperation::VerifyDownloadCompare(
    const base::Closure& continuation,
    const std::string& download_hash) {
  DCHECK_CURRENTLY_ON(BrowserThread::FILE);
  if (download_hash != hash_) {
    Error(error::kDownloadHashError);
    return;
  }

  BrowserThread::PostTask(
      BrowserThread::FILE,
      FROM_HERE,
      base::Bind(
          &WriteFromUrlOperation::VerifyDownloadComplete, this, continuation));
}
