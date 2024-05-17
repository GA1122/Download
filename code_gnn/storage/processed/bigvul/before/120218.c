void LayerTreeHost::DeleteContentsTexturesOnImplThread(
    ResourceProvider* resource_provider) {
  DCHECK(proxy_->IsImplThread());
  if (contents_texture_manager_)
    contents_texture_manager_->ClearAllMemory(resource_provider);
}
