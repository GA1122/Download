void LockContentsView::SwapActiveAuthBetweenPrimaryAndSecondary(
    bool is_primary) {
  if (Shell::Get()->login_screen_controller()->IsAuthenticating())
    return;

  if (is_primary && !primary_big_view_->IsAuthEnabled()) {
    LayoutAuth(primary_big_view_, opt_secondary_big_view_, true  );
    OnBigUserChanged();
  } else if (!is_primary && opt_secondary_big_view_ &&
             !opt_secondary_big_view_->IsAuthEnabled()) {
    LayoutAuth(opt_secondary_big_view_, primary_big_view_, true  );
    OnBigUserChanged();
  }
}
