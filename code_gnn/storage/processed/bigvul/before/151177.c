    DocumentThreadableLoaderFailedToStartLoadingForClient(
        ThreadableLoaderClient* client) {
  if (!client)
    return;
  if (client != pending_request_) {
    DCHECK(!pending_request_);
    return;
  }

  ClearPendingRequestData();
}
