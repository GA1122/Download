void StoragePartitionImpl::OpenLocalStorage(
    const url::Origin& origin,
    blink::mojom::StorageAreaRequest request) {
  int process_id = bindings_.dispatch_context();
  if (!ChildProcessSecurityPolicy::GetInstance()->CanAccessDataForOrigin(
          process_id, origin.GetURL())) {
    SYSLOG(WARNING) << "Killing renderer: illegal localStorage request.";
    bindings_.ReportBadMessage("Access denied for localStorage request");
    return;
  }
  dom_storage_context_->OpenLocalStorage(origin, std::move(request));
}
