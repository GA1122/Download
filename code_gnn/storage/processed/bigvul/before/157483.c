IsPinnedToTaskbarHelper::IsPinnedToTaskbarHelper(
    std::unique_ptr<service_manager::Connector> connector,
    const ErrorCallback& error_callback,
    const ResultCallback& result_callback)
    : connector_(std::move(connector)),
      error_callback_(error_callback),
      result_callback_(result_callback) {
  DCHECK(error_callback_);
  DCHECK(result_callback_);

  connector_->BindInterface(chrome::mojom::kUtilWinServiceName,
                            &shell_util_win_ptr_);
  shell_util_win_ptr_.set_connection_error_handler(base::Bind(
      &IsPinnedToTaskbarHelper::OnConnectionError, base::Unretained(this)));
  shell_util_win_ptr_->IsPinnedToTaskbar(
      base::Bind(&IsPinnedToTaskbarHelper::OnIsPinnedToTaskbarResult,
                 base::Unretained(this)));
}
