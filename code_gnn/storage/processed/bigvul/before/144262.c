bool IsSigninToAdd() {
  return LoginDisplayHost::default_host() &&
         user_manager::UserManager::Get()->IsUserLoggedIn();
}
