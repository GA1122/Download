void ResourceDispatcherHost::RemovePendingRequest(
    const PendingRequestList::iterator& iter) {
  ResourceDispatcherHostRequestInfo* info = InfoForRequest(iter->second);

  IncrementOutstandingRequestsMemoryCost(-1 * info->memory_cost(),
                                         info->child_id());

  if (info && info->login_handler())
    info->login_handler()->OnRequestCancelled();
  resource_queue_.RemoveRequest(iter->first);

  delete iter->second;
  pending_requests_.erase(iter);

  if (pending_requests_.empty())
    update_load_states_timer_.Stop();
}
