bool BrowserGpuChannelHostFactory::IsIOThread() {
  return BrowserThread::CurrentlyOn(BrowserThread::IO);
}
