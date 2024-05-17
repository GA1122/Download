void ResourceDispatcherHost::InsertIntoResourceQueue(
    URLRequest* request,
    const ResourceDispatcherHostRequestInfo& request_info) {
  resource_queue_.AddRequest(request, request_info);

  if (!update_load_states_timer_.IsRunning()) {
    update_load_states_timer_.Start(
        TimeDelta::FromMilliseconds(kUpdateLoadStatesIntervalMsec),
        this, &ResourceDispatcherHost::UpdateLoadStates);
  }
}
