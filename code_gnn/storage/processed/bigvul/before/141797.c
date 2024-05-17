void ChromeMetricsServiceClient::OnLogCleanShutdown() {
#if defined(OS_WIN)
  base::FilePath user_data_dir;
  if (base::PathService::Get(chrome::DIR_USER_DATA, &user_data_dir))
    browser_watcher::MarkOwnStabilityFileDeleted(user_data_dir);
#endif   
}
