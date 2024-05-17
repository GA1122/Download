void ConfirmInfoBar::ViewHierarchyChanged(bool is_add,
                                          views::View* parent,
                                          views::View* child) {
  if (is_add && child == this && !initialized_) {
    Init();
    initialized_ = true;
  }
  InfoBar::ViewHierarchyChanged(is_add, parent, child);
}
