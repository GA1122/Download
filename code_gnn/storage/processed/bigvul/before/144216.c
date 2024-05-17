void LockContentsView::SetAvatarForUser(const AccountId& account_id,
                                        const mojom::UserAvatarPtr& avatar) {
  auto replace = [&](const mojom::LoginUserInfoPtr& user) {
    auto changed = user->Clone();
    changed->basic_user_info->avatar = avatar->Clone();
    return changed;
  };

  LoginBigUserView* big =
      TryToFindBigUser(account_id, false  );
  if (big) {
    big->UpdateForUser(replace(big->GetCurrentUser()));
    return;
  }

  LoginUserView* user =
      users_list_ ? users_list_->GetUserView(account_id) : nullptr;
  if (user) {
    user->UpdateForUser(replace(user->current_user()), false  );
    return;
  }
}
