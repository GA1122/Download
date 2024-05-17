void BrowserView::SetBookmarkBarParent(views::View* new_parent) {

  if (new_parent == this) {
    views::View* target_view = nullptr;
    if (ui::MaterialDesignController::IsRefreshUi()) {
      target_view = infobar_container_;
    } else {
      target_view = top_container_;
    }
    const int target_index = GetIndexOf(target_view);
    DCHECK_GE(target_index, 0);
    AddChildViewAt(bookmark_bar_view_.get(), target_index);
  } else if (new_parent == top_container_) {

    if (ui::MaterialDesignController::IsRefreshUi()) {
      new_parent->AddChildView(bookmark_bar_view_.get());
    } else {
      new_parent->AddChildViewAt(bookmark_bar_view_.get(), 0);
    }
  } else {
    DCHECK(!new_parent);
    bookmark_bar_view_->parent()->RemoveChildView(bookmark_bar_view_.get());
  }
}
