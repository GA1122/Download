bool HasAuthError(Profile* profile) {
  const SigninErrorController* error =
      SigninErrorControllerFactory::GetForProfile(profile);
  return error && error->HasError();
}
