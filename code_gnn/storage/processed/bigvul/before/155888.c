std::string GetAuthErrorAccountId(Profile* profile) {
  const SigninErrorController* error =
      SigninErrorControllerFactory::GetForProfile(profile);
  if (!error)
    return std::string();

  return error->error_account_id();
}
