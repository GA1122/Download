void ServiceWorkerHandler::StopAllWorkers(
    std::unique_ptr<StopAllWorkersCallback> callback) {
  if (!enabled_) {
    callback->sendFailure(CreateDomainNotEnabledErrorResponse());
    return;
  }
  if (!context_) {
    callback->sendFailure(CreateContextErrorResponse());
    return;
  }
  context_->StopAllServiceWorkers(base::BindOnce(
      &StopAllWorkersCallback::sendSuccess, std::move(callback)));
}
