gfx::Size LayerTreeHost::GetUIResourceSize(UIResourceId uid) const {
  UIResourceClientMap::const_iterator iter = ui_resource_client_map_.find(uid);
  if (iter == ui_resource_client_map_.end())
    return gfx::Size();

  const UIResourceClientData& data = iter->second;
  return data.size;
}
