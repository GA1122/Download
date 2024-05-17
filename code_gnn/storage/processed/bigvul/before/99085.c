const ResourceDispatcherHostRequestInfo* ResourceDispatcherHost::InfoForRequest(
    const URLRequest* request) {
  const ResourceDispatcherHostRequestInfo* info =
      static_cast<const ResourceDispatcherHostRequestInfo*>(
          request->GetUserData(NULL));
  DLOG_IF(WARNING, !info) << "Request doesn't seem to have our data";
  return info;
}
