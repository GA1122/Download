void OobeUI::OnScreenAssetsLoaded(const std::string& async_assets_load_id) {
  DCHECK(!async_assets_load_id.empty());

  for (size_t i = 0; i < handlers_.size(); ++i) {
    if (handlers_[i]->async_assets_load_id() == async_assets_load_id)
      handlers_[i]->InitializeBase();
  }
}
