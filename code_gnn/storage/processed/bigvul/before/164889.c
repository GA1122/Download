void DownloadResourceHandler::CancelRequest() {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);

  const ResourceRequestInfoImpl* info = GetRequestInfo();
  ResourceDispatcherHostImpl::Get()->CancelRequest(
      info->GetChildID(),
      info->GetRequestID());
}
