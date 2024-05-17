views::View* AutofillDialogViews::GetInitiallyFocusedView() {
  if (!window_ || !focus_manager_)
    return NULL;

  if (sign_in_web_view_->visible())
    return sign_in_web_view_;

  if (loading_shield_->visible())
    return views::DialogDelegateView::GetInitiallyFocusedView();

  DCHECK(scrollable_area_->visible());

  views::FocusManager* manager = focus_manager_;
  for (views::View* next = scrollable_area_;
       next;
       next = manager->GetNextFocusableView(next, window_, false, true)) {
    views::View* input_view = GetAncestralInputView(next);
    if (!input_view)
      continue;

    if (validity_map_.empty() ||
        validity_map_.find(input_view) != validity_map_.end()) {
      return next;
    }
  }

  return views::DialogDelegateView::GetInitiallyFocusedView();
}
