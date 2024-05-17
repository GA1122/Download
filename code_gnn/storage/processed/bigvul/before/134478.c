void HomePageUndoBubble::WindowClosing() {
  DCHECK_EQ(this, home_page_undo_bubble_);
  home_page_undo_bubble_ = NULL;
}
