GamepadProvider::~GamepadProvider() {
  GamepadDataFetcherManager::GetInstance()->ClearProvider();

  base::SystemMonitor* monitor = base::SystemMonitor::Get();
  if (monitor)
    monitor->RemoveDevicesChangedObserver(this);

  polling_thread_->task_runner()->PostTask(
      FROM_HERE, base::Bind(&GamepadFetcherVector::clear,
                            base::Unretained(&data_fetchers_)));

  polling_thread_->Stop();

  DCHECK(data_fetchers_.empty());
}
