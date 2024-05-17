void CrostiniUpgrader::OnBackup(CrostiniResult result) {
  if (result != CrostiniResult::SUCCESS) {
    for (auto& observer : upgrader_observers_) {
      observer.OnBackupFailed();
    }
    return;
  }
  for (auto& observer : upgrader_observers_) {
    observer.OnBackupSucceeded();
  }
}
