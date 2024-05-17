void InfoBar::ViewHierarchyChanged(bool is_add, views::View* parent,
                                   views::View* child) {
  if (child == this) {
    if (is_add) {
      InfoBarAdded();
    } else {
      InfoBarRemoved();
    }
  }

  if (GetWidget() && GetWidget()->IsAccessibleWidget()) {
    if (parent == this && child != close_button_ &&
        HasChildView(close_button_) &&
        GetChildViewAt(GetChildViewCount() - 1) != close_button_) {
      RemoveChildView(close_button_);
      AddChildView(close_button_);
    }

    close_button_->SetFocusable(true);
  }
}
