DownloadController* DownloadController::GetInstance() {
  return base::Singleton<DownloadController>::get();
}
