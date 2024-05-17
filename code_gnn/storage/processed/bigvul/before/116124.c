void ResourceDispatcherHostImpl::CancelRequestsForContext(
    ResourceContext* context) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));
  DCHECK(context);

  CHECK(ContainsKey(active_resource_contexts_, context));

  std::vector<net::URLRequest*> requests_to_cancel;
  for (PendingRequestList::iterator i = pending_requests_.begin();
       i != pending_requests_.end();) {
    ResourceRequestInfoImpl* info =
        ResourceRequestInfoImpl::ForRequest(i->second);
    if (info->GetContext() == context) {
      requests_to_cancel.push_back(i->second);
      pending_requests_.erase(i++);
    } else {
      ++i;
    }
  }

  for (BlockedRequestMap::iterator i = blocked_requests_map_.begin();
       i != blocked_requests_map_.end();) {
    BlockedRequestsList* requests = i->second;
    if (requests->empty()) {
      ++i;
      continue;
    }
    ResourceRequestInfoImpl* info =
        ResourceRequestInfoImpl::ForRequest(requests->front());
    if (info->GetContext() == context) {
      blocked_requests_map_.erase(i++);
      for (BlockedRequestsList::const_iterator it = requests->begin();
           it != requests->end(); ++it) {
        net::URLRequest* request = *it;
        info = ResourceRequestInfoImpl::ForRequest(request);
        DCHECK_EQ(context, info->GetContext());
        IncrementOutstandingRequestsMemoryCost(-1 * info->memory_cost(),
                                               info->GetChildID());
        requests_to_cancel.push_back(request);
      }
      delete requests;
    } else {
      ++i;
    }
  }

  for (std::vector<net::URLRequest*>::iterator i = requests_to_cancel.begin();
       i != requests_to_cancel.end(); ++i) {
    net::URLRequest* request = *i;
    ResourceRequestInfoImpl* info =
        ResourceRequestInfoImpl::ForRequest(request);
    GlobalRequestID request_id(info->GetChildID(), info->GetRequestID());
    bool is_transferred = IsTransferredNavigation(request_id);
    DCHECK(info->is_download() || is_transferred);
    if (is_transferred)
      transferred_navigations_.erase(request_id);
    delete request;
  }

  for (PendingRequestList::const_iterator i = pending_requests_.begin();
       i != pending_requests_.end(); ++i) {
    ResourceRequestInfoImpl* info =
        ResourceRequestInfoImpl::ForRequest(i->second);
    CHECK_NE(info->GetContext(), context);
  }

  for (BlockedRequestMap::const_iterator i = blocked_requests_map_.begin();
       i != blocked_requests_map_.end(); ++i) {
    BlockedRequestsList* requests = i->second;
    if (!requests->empty()) {
      ResourceRequestInfoImpl* info =
          ResourceRequestInfoImpl::ForRequest(requests->front());
      CHECK_NE(info->GetContext(), context);
    }
  }
}
