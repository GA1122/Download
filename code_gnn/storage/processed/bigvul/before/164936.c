bool ResourceDispatcherHostImpl::HandleExternalProtocol(ResourceLoader* loader,
                                                        const GURL& url) {
  ResourceRequestInfoImpl* info = loader->GetRequestInfo();

  if (!IsResourceTypeFrame(info->GetResourceType()))
    return false;

  net::URLRequest* url_request = loader->request();
  DCHECK(url_request);

  const net::URLRequestJobFactory* job_factory =
      url_request->context()->job_factory();
  if (!url.is_valid() || job_factory->IsHandledProtocol(url.scheme()))
    return false;

  return GetContentClient()->browser()->HandleExternalProtocol(
      url, info->GetWebContentsGetterForRequest(), info->GetChildID(),
      info->GetNavigationUIData(), info->IsMainFrame(),
      info->GetPageTransition(), info->HasUserGesture(), url_request->method(),
      url_request->extra_request_headers());
}
