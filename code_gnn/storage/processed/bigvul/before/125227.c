base::TaskRunner* RenderMessageFilter::OverrideTaskRunnerForMessage(
    const IPC::Message& message) {
#if defined(OS_WIN)
  if (message.type() == ViewHostMsg_GetMonitorColorProfile::ID)
    return BrowserThread::GetBlockingPool();
#endif
  return NULL;
}
