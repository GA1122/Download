void ResourceDispatcherHost::UpdateLoadStates() {
  LoadInfoMap info_map;

  PendingRequestList::const_iterator i;

  std::map<std::pair<int, int>, uint64> largest_upload_size;
  for (i = pending_requests_.begin(); i != pending_requests_.end(); ++i) {
    URLRequest* request = i->second;
    ResourceDispatcherHostRequestInfo* info = InfoForRequest(request);
    uint64 upload_size = info->upload_size();
    if (request->GetLoadState() != net::LOAD_STATE_SENDING_REQUEST)
      upload_size = 0;
    std::pair<int, int> key(info->child_id(), info->route_id());
    if (upload_size && largest_upload_size[key] < upload_size)
      largest_upload_size[key] = upload_size;
  }

  for (i = pending_requests_.begin(); i != pending_requests_.end(); ++i) {
    URLRequest* request = i->second;
    net::LoadState load_state = request->GetLoadState();
    ResourceDispatcherHostRequestInfo* info = InfoForRequest(request);

    bool update_upload_progress = MaybeUpdateUploadProgress(info, request);

    if (info->last_load_state() != load_state || update_upload_progress) {
      std::pair<int, int> key(info->child_id(), info->route_id());

      if (largest_upload_size.find(key) != largest_upload_size.end() &&
          info->upload_size() < largest_upload_size[key])
        continue;

      info->set_last_load_state(load_state);

      net::LoadState to_insert;
      LoadInfoMap::iterator existing = info_map.find(key);
      if (existing == info_map.end()) {
        to_insert = load_state;
      } else {
        to_insert =
            MoreInterestingLoadState(existing->second.load_state, load_state);
        if (to_insert == existing->second.load_state)
          continue;
      }
      LoadInfo& load_info = info_map[key];
      load_info.url = request->url();
      load_info.load_state = to_insert;
      load_info.upload_size = info->upload_size();
      load_info.upload_position = request->GetUploadProgress();
    }
  }

  if (info_map.empty())
    return;

  LoadInfoUpdateTask* task = new LoadInfoUpdateTask;
  task->info_map.swap(info_map);
  ChromeThread::PostTask(ChromeThread::UI, FROM_HERE, task);
}
