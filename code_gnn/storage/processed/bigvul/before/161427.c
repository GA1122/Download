Response ServiceWorkerHandler::StopWorker(const std::string& version_id) {
  if (!enabled_)
    return CreateDomainNotEnabledErrorResponse();
  if (!context_)
    return CreateContextErrorResponse();
  int64_t id = 0;
  if (!base::StringToInt64(version_id, &id))
    return CreateInvalidVersionIdErrorResponse();
  BrowserThread::PostTask(BrowserThread::IO, FROM_HERE,
                          base::BindOnce(&StopServiceWorkerOnIO, context_, id));
  return Response::OK();
}
