 bool ResourceDispatcherHost::HandleExternalProtocol(int request_id,
                                                    int child_id,
                                                    int route_id,
                                                    const GURL& url,
                                                    ResourceType::Type type,
                                                    ResourceHandler* handler) {
  if (!ResourceType::IsFrame(type) || URLRequest::IsHandledURL(url))
    return false;

  ChromeThread::PostTask(
      ChromeThread::UI, FROM_HERE,
      NewRunnableFunction(
          &ExternalProtocolHandler::LaunchUrl, url, child_id, route_id));

  handler->OnResponseCompleted(request_id, URLRequestStatus(
                                               URLRequestStatus::FAILED,
                                               net::ERR_ABORTED),
                               std::string());   
  return true;
}
