BrowserGpuChannelHostFactory::GetIOLoopProxy() {
  return BrowserThread::GetMessageLoopProxyForThread(BrowserThread::IO);
}
