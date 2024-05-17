Response ServiceWorkerHandler::StartWorker(const std::string& scope_url) {
  if (!enabled_)
    return CreateDomainNotEnabledErrorResponse();
  if (!context_)
    return CreateContextErrorResponse();
  context_->StartServiceWorker(GURL(scope_url), base::Bind(&StatusNoOp));
  return Response::OK();
}
