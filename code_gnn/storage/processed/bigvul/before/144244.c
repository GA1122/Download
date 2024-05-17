bool CanRemoveUser(const user_manager::User* user) {
  const bool is_single_user =
      user_manager::UserManager::Get()->GetUsers().size() == 1;

  if (is_single_user && !IsEnterpriseManaged())
    return false;
  if (!user->GetAccountId().is_valid())
    return false;
  if (user->GetAccountId() == GetOwnerAccountId())
    return false;
  if (user->GetType() == user_manager::USER_TYPE_PUBLIC_ACCOUNT ||
      user->is_logged_in() || IsSigninToAdd())
    return false;

  return true;
}
