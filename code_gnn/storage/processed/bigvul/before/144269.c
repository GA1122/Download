void UserSelectionScreen::OnUserStatusChecked(
    const AccountId& account_id,
    TokenHandleUtil::TokenHandleStatus status) {
  if (status == TokenHandleUtil::INVALID) {
    RecordReauthReason(account_id, ReauthReason::INVALID_TOKEN_HANDLE);
    token_handle_util_->MarkHandleInvalid(account_id);
    SetAuthType(account_id, proximity_auth::mojom::AuthType::ONLINE_SIGN_IN,
                base::string16());
  }
}
