void ResourceDispatcherHostImpl::UpdateLoadStates() {
  LoadInfoMap info_map;

  LoaderMap::const_iterator i;

  std::map<std::pair<int, int>, uint64> largest_upload_size;
  for (i = pending_loaders_.begin(); i != pending_loaders_.end(); ++i) {
    net::URLRequest* request = i->second->request();
    ResourceRequestInfoImpl* info = i->second->GetRequestInfo();
    uint64 upload_size = request->GetUploadProgress().size();
    if (request->GetLoadState().state != net::LOAD_STATE_SENDING_REQUEST)
      upload_size = 0;
    std::pair<int, int> key(info->GetChildID(), info->GetRouteID());
    if (upload_size && largest_upload_size[key] < upload_size)
      largest_upload_size[key] = upload_size;
  }

  for (i = pending_loaders_.begin(); i != pending_loaders_.end(); ++i) {
    net::URLRequest* request = i->second->request();
    ResourceRequestInfoImpl* info = i->second->GetRequestInfo();
    net::LoadStateWithParam load_state = request->GetLoadState();
    net::UploadProgress progress = request->GetUploadProgress();

    i->second->ReportUploadProgress();

    std::pair<int, int> key(info->GetChildID(), info->GetRouteID());

    if (largest_upload_size.find(key) != largest_upload_size.end() &&
        progress.size() < largest_upload_size[key])
      continue;

    net::LoadStateWithParam to_insert = load_state;
    LoadInfoMap::iterator existing = info_map.find(key);
    if (existing != info_map.end()) {
      to_insert =
          MoreInterestingLoadState(existing->second.load_state, load_state);
      if (to_insert.state == existing->second.load_state.state)
        continue;
    }
    LoadInfo& load_info = info_map[key];
    load_info.url = request->url();
    load_info.load_state = to_insert;
    load_info.upload_size = progress.size();
    load_info.upload_position = progress.position();
  }

  if (info_map.empty())
    return;

  BrowserThread::PostTask(
      BrowserThread::UI, FROM_HERE,
      base::Bind(&LoadInfoUpdateCallback, info_map));
}
