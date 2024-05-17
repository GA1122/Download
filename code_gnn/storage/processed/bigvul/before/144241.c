void UserSelectionScreen::AttemptEasySignin(const AccountId& account_id,
                                            const std::string& secret,
                                            const std::string& key_label) {
  DCHECK_EQ(GetScreenType(), SIGNIN_SCREEN);

  const user_manager::User* const user =
      user_manager::UserManager::Get()->FindUser(account_id);
  DCHECK(user);
  UserContext user_context(*user);
  user_context.SetAuthFlow(UserContext::AUTH_FLOW_EASY_UNLOCK);
  user_context.SetKey(Key(secret));
  user_context.GetKey()->SetLabel(key_label);

  if (LoginDisplayHost::default_host()) {
    LoginDisplayHost::default_host()->GetLoginDisplay()->delegate()->Login(
        user_context, SigninSpecifics());
  }
}
