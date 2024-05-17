bool ResourceDispatcherHostImpl::HandleExternalProtocol(
    int request_id,
    int child_id,
    int route_id,
    const GURL& url,
    ResourceType::Type type,
    const net::URLRequestJobFactory& job_factory,
    ResourceHandler* handler) {
  if (!ResourceType::IsFrame(type) ||
      job_factory.IsHandledURL(url)) {
    return false;
  }

  if (delegate_)
    delegate_->HandleExternalProtocol(url, child_id, route_id);

  handler->OnResponseCompleted(
      request_id,
      net::URLRequestStatus(net::URLRequestStatus::FAILED,
                            net::ERR_UNKNOWN_URL_SCHEME),
      std::string());   
  return true;
}
