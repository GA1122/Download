void LoadInfoUpdateCallback(const LoadInfoMap& info_map) {
  LoadInfoMap::const_iterator i;
  for (i = info_map.begin(); i != info_map.end(); ++i) {
    RenderViewHostImpl* view =
        RenderViewHostImpl::FromID(i->first.first, i->first.second);
    if (view)   
      view->LoadStateChanged(i->second.url, i->second.load_state,
                             i->second.upload_position,
                             i->second.upload_size);
  }
}
