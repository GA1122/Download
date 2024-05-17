Response ServiceWorkerHandler::InspectWorker(const std::string& version_id) {
  if (!enabled_)
    return CreateDomainNotEnabledErrorResponse();
  if (!context_)
    return CreateContextErrorResponse();

  int64_t id = blink::mojom::kInvalidServiceWorkerVersionId;
  if (!base::StringToInt64(version_id, &id))
    return CreateInvalidVersionIdErrorResponse();
  BrowserThread::PostTask(
      BrowserThread::IO, FROM_HERE,
      base::BindOnce(&GetDevToolsRouteInfoOnIO, context_, id,
                     base::Bind(&ServiceWorkerHandler::OpenNewDevToolsWindow,
                                weak_factory_.GetWeakPtr())));
  return Response::OK();
}
