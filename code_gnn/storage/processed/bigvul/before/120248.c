void LayerTreeHost::RecreateUIResources() {
  for (UIResourceClientMap::iterator iter = ui_resource_client_map_.begin();
       iter != ui_resource_client_map_.end();
       ++iter) {
    UIResourceId uid = iter->first;
    const UIResourceClientData& data = iter->second;
    bool resource_lost = true;
    UIResourceRequest request(UIResourceRequest::UIResourceCreate,
                              uid,
                              data.client->GetBitmap(uid, resource_lost));
    ui_resource_request_queue_.push_back(request);
  }
}
