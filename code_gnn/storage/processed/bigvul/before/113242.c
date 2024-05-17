bool ShelfLayoutManager::SetAlignment(ShelfAlignment alignment) {
  if (alignment_ == alignment)
    return false;

  alignment_ = alignment;
  if (launcher_)
    launcher_->SetAlignment(alignment);
  StatusAreaWidget* status_area_widget =
      Shell::GetInstance()->status_area_widget();
  if (status_area_widget)
    Shell::GetInstance()->status_area_widget()->SetShelfAlignment(alignment);
  LayoutShelf();
  return true;
}
