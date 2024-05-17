LoginBigUserView* LockContentsView::CurrentBigUserView() {
  if (opt_secondary_big_view_ && opt_secondary_big_view_->IsAuthEnabled()) {
    DCHECK(!primary_big_view_->IsAuthEnabled());
    return opt_secondary_big_view_;
  }

  return primary_big_view_;
}
