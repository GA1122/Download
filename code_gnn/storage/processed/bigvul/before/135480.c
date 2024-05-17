base::string16 ReplaceEmptyUsername(const base::string16& username) {
  if (username.empty())
    return l10n_util::GetStringUTF16(IDS_PASSWORD_MANAGER_EMPTY_LOGIN);
  return username;
}
