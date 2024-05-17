void GamepadProvider::AddGamepadDataFetcher(
    std::unique_ptr<GamepadDataFetcher> fetcher) {
  polling_thread_->task_runner()->PostTask(
      FROM_HERE, base::Bind(&GamepadProvider::DoAddGamepadDataFetcher,
                            base::Unretained(this), base::Passed(&fetcher)));
}
