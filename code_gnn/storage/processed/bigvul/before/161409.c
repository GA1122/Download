 Response ServiceWorkerHandler::Enable() {
  if (enabled_)
    return Response::OK();
  if (!context_)
    return CreateContextErrorResponse();
  enabled_ = true;

  context_watcher_ = new ServiceWorkerContextWatcher(
      context_, base::Bind(&ServiceWorkerHandler::OnWorkerRegistrationUpdated,
                           weak_factory_.GetWeakPtr()),
      base::Bind(&ServiceWorkerHandler::OnWorkerVersionUpdated,
                 weak_factory_.GetWeakPtr()),
      base::Bind(&ServiceWorkerHandler::OnErrorReported,
                 weak_factory_.GetWeakPtr()));
  context_watcher_->Start();

  return Response::OK();
}
