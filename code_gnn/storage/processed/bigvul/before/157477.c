void GetIsPinnedToTaskbarState(
    std::unique_ptr<service_manager::Connector> connector,
    const ConnectionErrorCallback& on_error_callback,
    const IsPinnedToTaskbarCallback& result_callback) {
  IsPinnedToTaskbarHelper::GetState(std::move(connector), on_error_callback,
                                    result_callback);
}
