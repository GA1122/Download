void BrowserView::ViewHierarchyChanged(bool is_add,
                                       views::View* parent,
                                       views::View* child) {
  if (!initialized_ && is_add && child == this && GetWidget()) {
    Init();
    initialized_ = true;
  }
}
