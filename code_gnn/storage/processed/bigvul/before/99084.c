ResourceDispatcherHostRequestInfo* ResourceDispatcherHost::InfoForRequest(
    URLRequest* request) {
  const URLRequest* const_request = request;
  return const_cast<ResourceDispatcherHostRequestInfo*>(
      InfoForRequest(const_request));
}
