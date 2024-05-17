void InfoBarContainer::ViewHierarchyChanged(bool is_add,
                                            views::View* parent,
                                            views::View* child) {
  if (parent == this && child->GetParent() == this) {
    if (delegate_) {
      delegate_->InfoBarContainerSizeChanged(false);
    }
  }
}
