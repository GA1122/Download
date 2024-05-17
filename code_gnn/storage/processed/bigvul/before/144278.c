bool UserSelectionScreen::ShouldForceOnlineSignIn(
    const user_manager::User* user) {
  if (user->is_logged_in())
    return false;

  const user_manager::User::OAuthTokenStatus token_status =
      user->oauth_token_status();
  const bool is_supervised_user =
      user->GetType() == user_manager::USER_TYPE_SUPERVISED;
  const bool is_public_session =
      user->GetType() == user_manager::USER_TYPE_PUBLIC_ACCOUNT;
  const bool has_gaia_account = user->HasGaiaAccount();

  if (is_supervised_user)
    return false;

  if (is_public_session)
    return false;

  if (has_gaia_account &&
      token_status == user_manager::User::OAUTH2_TOKEN_STATUS_INVALID)
    RecordReauthReason(user->GetAccountId(), ReauthReason::OTHER);

  return user->force_online_signin() || !user->profile_ever_initialized() ||
         (has_gaia_account &&
          (token_status == user_manager::User::OAUTH2_TOKEN_STATUS_INVALID ||
           token_status == user_manager::User::OAUTH_TOKEN_STATUS_UNKNOWN));
}
