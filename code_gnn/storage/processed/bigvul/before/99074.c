void ResourceDispatcherHost::CancelRequest(int child_id,
                                           int request_id,
                                           bool from_renderer,
                                           bool allow_delete) {
  PendingRequestList::iterator i = pending_requests_.find(
      GlobalRequestID(child_id, request_id));
  if (i == pending_requests_.end()) {
    DLOG(WARNING) << "Canceling a request that wasn't found";
    return;
  }

  ResourceDispatcherHostRequestInfo* info = InfoForRequest(i->second);
  if (!from_renderer || !info->is_download()) {
    if (info->login_handler()) {
      info->login_handler()->OnRequestCancelled();
      info->set_login_handler(NULL);
    }
    if (info->ssl_client_auth_handler()) {
      info->ssl_client_auth_handler()->OnRequestCancelled();
      info->set_ssl_client_auth_handler(NULL);
    }
    if (!i->second->is_pending() && allow_delete) {
      RemovePendingRequest(info->child_id(), info->request_id());
    } else {
      i->second->Cancel();
    }
  }

}
