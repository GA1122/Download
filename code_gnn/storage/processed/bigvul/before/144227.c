void LockContentsView::UpdateAuthForPublicAccount(
    LoginPublicAccountUserView* opt_to_update,
    LoginPublicAccountUserView* opt_to_hide,
    bool animate) {
  if (opt_to_update)
    opt_to_update->SetAuthEnabled(true  , animate);
  if (opt_to_hide)
    opt_to_hide->SetAuthEnabled(false  , animate);
}
