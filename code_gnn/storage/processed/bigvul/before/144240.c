bool AllowFingerprintForUser(const user_manager::User* user) {
  if (!user->is_logged_in())
    return false;

  quick_unlock::QuickUnlockStorage* quick_unlock_storage =
      quick_unlock::QuickUnlockFactory::GetForUser(user);
  if (!quick_unlock_storage)
    return false;

  return quick_unlock_storage->IsFingerprintAuthenticationAvailable();
}
