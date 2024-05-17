void ShelfLayoutManager::PrepareForShutdown() {
  in_shutdown_ = true;
  Shell::Get()->activation_client()->RemoveObserver(this);
}
