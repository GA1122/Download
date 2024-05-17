PageInfoWindowView::~PageInfoWindowView() {
  DCHECK(opened_window_count_ > 0);
  opened_window_count_--;
}
