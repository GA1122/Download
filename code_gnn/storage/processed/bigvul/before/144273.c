void UserSelectionScreen::SetAuthType(const AccountId& account_id,
                                      proximity_auth::mojom::AuthType auth_type,
                                      const base::string16& initial_value) {
  if (GetAuthType(account_id) ==
      proximity_auth::mojom::AuthType::FORCE_OFFLINE_PASSWORD) {
    return;
  }

  DCHECK(GetAuthType(account_id) !=
             proximity_auth::mojom::AuthType::FORCE_OFFLINE_PASSWORD ||
         auth_type == proximity_auth::mojom::AuthType::FORCE_OFFLINE_PASSWORD);
  user_auth_type_map_[account_id] = auth_type;
  view_->SetAuthType(account_id, auth_type, initial_value);
}
