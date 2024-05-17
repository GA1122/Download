void DocumentLoader::ClearMainResourceHandle() {
  if (!main_resource_)
    return;
  main_resource_->RemoveClient(this);
  main_resource_ = nullptr;
}
