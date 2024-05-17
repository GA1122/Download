 void HostPortAllocatorSession::OnSessionRequestDone(
    UrlFetcher* url_fetcher,
    const net::URLRequestStatus& status,
    int response_code,
    const std::string& response) {
  url_fetchers_.erase(url_fetcher);
  delete url_fetcher;
// void HostPortAllocatorSession::OnURLFetchComplete(
//     const net::URLFetcher* source) {
//   url_fetchers_.erase(source);
//   delete source;
  
  if (response_code != net::HTTP_OK) {
//   if (source->GetResponseCode() != net::HTTP_OK) {
      LOG(WARNING) << "Received error when allocating relay session: "
                 << response_code;
//                  << source->GetResponseCode();
      TryCreateRelaySession();
      return;
    }
  
//   std::string response;
//   source->GetResponseAsString(&response);
    ReceiveSessionResponse(response);
  }