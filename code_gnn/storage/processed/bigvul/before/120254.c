bool LayerTreeHost::RequestPartialTextureUpdate() {
  if (partial_texture_update_requests_ >= MaxPartialTextureUpdates())
    return false;

  partial_texture_update_requests_++;
  return true;
}
