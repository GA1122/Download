bool IsUserAllowedForARC(const AccountId& account_id) {
  return user_manager::UserManager::IsInitialized() &&
         arc::IsArcAllowedForUser(
             user_manager::UserManager::Get()->FindUser(account_id));
}
