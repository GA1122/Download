void LockContentsView::OnPublicAccountTapped() {
  expanded_view_->UpdateForUser(CurrentBigUserView()->GetCurrentUser());
  SetDisplayStyle(DisplayStyle::kExclusivePublicAccountExpandedView);
}
