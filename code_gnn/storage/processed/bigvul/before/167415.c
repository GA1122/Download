DevToolsDownloadManagerDelegate::GetInstance() {
  if (!g_devtools_manager_delegate)
    new DevToolsDownloadManagerDelegate();

  return g_devtools_manager_delegate;
}
