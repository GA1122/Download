LoginBigUserView* LockContentsView::TryToFindBigUser(const AccountId& user,
                                                     bool require_auth_active) {
  LoginBigUserView* view = nullptr;

  if (primary_big_view_ &&
      primary_big_view_->GetCurrentUser()->basic_user_info->account_id ==
          user) {
    view = primary_big_view_;
  } else if (opt_secondary_big_view_ &&
             opt_secondary_big_view_->GetCurrentUser()
                     ->basic_user_info->account_id == user) {
    view = opt_secondary_big_view_;
  }

  if (require_auth_active && view && !view->IsAuthEnabled())
    view = nullptr;

  return view;
}
