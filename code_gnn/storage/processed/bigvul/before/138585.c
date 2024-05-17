void WriteFromUrlOperation::VerifyDownloadComplete(
    const base::Closure& continuation) {
  DCHECK_CURRENTLY_ON(BrowserThread::FILE);
  if (IsCancelled()) {
    return;
  }

  SetProgress(kProgressComplete);
  BrowserThread::PostTask(BrowserThread::FILE, FROM_HERE, continuation);
}
