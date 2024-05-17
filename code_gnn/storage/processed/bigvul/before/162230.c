GamepadProvider::GamepadProvider(
    GamepadConnectionChangeClient* connection_change_client,
    std::unique_ptr<GamepadDataFetcher> fetcher)
    : is_paused_(true),
      have_scheduled_do_poll_(false),
      devices_changed_(true),
      ever_had_user_gesture_(false),
      sanitize_(true),
      gamepad_shared_buffer_(new GamepadSharedBuffer()),
      connection_change_client_(connection_change_client) {
  Initialize(std::move(fetcher));
}
