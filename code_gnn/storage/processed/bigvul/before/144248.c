proximity_auth::mojom::AuthType UserSelectionScreen::GetAuthType(
    const AccountId& account_id) const {
  if (user_auth_type_map_.find(account_id) == user_auth_type_map_.end())
    return proximity_auth::mojom::AuthType::OFFLINE_PASSWORD;
  return user_auth_type_map_.find(account_id)->second;
}
