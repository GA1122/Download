UIResourceId LayerTreeHost::CreateUIResource(UIResourceClient* client) {
  DCHECK(client);

  UIResourceId next_id = next_ui_resource_id_++;
  DCHECK(ui_resource_client_map_.find(next_id) ==
         ui_resource_client_map_.end());

  bool resource_lost = false;
  UIResourceRequest request(UIResourceRequest::UIResourceCreate,
                            next_id,
                            client->GetBitmap(next_id, resource_lost));
  ui_resource_request_queue_.push_back(request);

  UIResourceClientData data;
  data.client = client;
  data.size = request.GetBitmap().GetSize();

  ui_resource_client_map_[request.GetId()] = data;
  return request.GetId();
}
