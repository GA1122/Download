void ResourceDispatcherHost::SetRequestInfo(
    URLRequest* request,
    ResourceDispatcherHostRequestInfo* info) {
  request->SetUserData(NULL, info);
}
