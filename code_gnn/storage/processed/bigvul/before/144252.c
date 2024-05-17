AccountId GetOwnerAccountId() {
  std::string owner_email;
  chromeos::CrosSettings::Get()->GetString(chromeos::kDeviceOwner,
                                           &owner_email);
  const AccountId owner = user_manager::known_user::GetAccountId(
      owner_email, std::string()  , AccountType::UNKNOWN);
  return owner;
}
