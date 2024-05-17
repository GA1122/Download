void GamepadProvider::DoAddGamepadDataFetcher(
    std::unique_ptr<GamepadDataFetcher> fetcher) {
  DCHECK(polling_thread_->task_runner()->BelongsToCurrentThread());

  if (!fetcher)
    return;

  InitializeDataFetcher(fetcher.get());
  data_fetchers_.push_back(std::move(fetcher));
}
