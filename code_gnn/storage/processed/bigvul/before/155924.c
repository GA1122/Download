base::Value GetAccountValue(const AccountInfo& account,
                            AccountTrackerService* account_tracker) {
  DCHECK(!account.IsEmpty());
  base::Value dictionary(base::Value::Type::DICTIONARY);
  dictionary.SetKey("email", base::Value(account.email));
  dictionary.SetKey("fullName", base::Value(account.full_name));
  dictionary.SetKey("givenName", base::Value(account.given_name));
  const gfx::Image& account_image =
      account_tracker->GetAccountImage(account.account_id);
  if (!account_image.IsEmpty()) {
    dictionary.SetKey(
        "avatarImage",
        base::Value(webui::GetBitmapDataUrl(account_image.AsBitmap())));
  }
  return dictionary;
}
