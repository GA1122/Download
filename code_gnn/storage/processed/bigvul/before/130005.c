void ChromeBrowserMainParts::StartMetricsRecording() {
  TRACE_EVENT0("startup", "ChromeBrowserMainParts::StartMetricsRecording");

  g_browser_process->metrics_service()->CheckForClonedInstall(
      BrowserThread::GetMessageLoopProxyForThread(BrowserThread::FILE));

  g_browser_process->GetMetricsServicesManager()->UpdateUploadPermissions(true);
}
