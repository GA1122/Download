views::View* ShelfWidget::DelegateView::GetDefaultFocusableChild() {
  if (IsUsingViewsShelf())
    return FindFirstOrLastFocusableChild(shelf_widget_->login_shelf_view_,
                                         default_last_focusable_child_);
  return GetFirstFocusableChild();
}
