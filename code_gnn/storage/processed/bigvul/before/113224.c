void ShelfLayoutManager::GetShelfSize(int* width, int* height) {
  *width = *height = 0;
  gfx::Rect status_bounds(status_->GetWindowScreenBounds());
  gfx::Size launcher_size = launcher_ ?
      launcher_widget()->GetContentsView()->GetPreferredSize() : gfx::Size();
  if (alignment_ == SHELF_ALIGNMENT_BOTTOM)
    *height = std::max(launcher_size.height(), status_bounds.height());
  else
    *width = std::max(launcher_size.width(), status_bounds.width());
}
