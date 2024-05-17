void LayerTreeHost::DeleteUIResource(UIResourceId uid) {
  UIResourceClientMap::iterator iter = ui_resource_client_map_.find(uid);
  if (iter == ui_resource_client_map_.end())
    return;

  UIResourceRequest request(UIResourceRequest::UIResourceDelete, uid);
  ui_resource_request_queue_.push_back(request);
  ui_resource_client_map_.erase(iter);
}
