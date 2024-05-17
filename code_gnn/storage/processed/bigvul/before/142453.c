void ShelfLayoutManager::LayoutShelf() {
  if (in_shutdown_)
    return;

  LayoutShelfAndUpdateBounds();
}
