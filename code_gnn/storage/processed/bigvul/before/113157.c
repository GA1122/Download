void Launcher::DelegateView::Layout() {
  if (child_count() == 0)
    return;
  if (launcher_->alignment_ == SHELF_ALIGNMENT_BOTTOM) {
    int w = std::max(0, width() - launcher_->status_size_.width());
    child_at(0)->SetBounds(0, 0, w, height());
  } else {
    int h = std::max(0, height() - launcher_->status_size_.height());
    child_at(0)->SetBounds(0, 0, width(), h);
  }
}
