bool PeopleHandler::IsProfileAuthNeededOrHasErrors() {
  return !SigninManagerFactory::GetForProfile(profile_)->IsAuthenticated() ||
         SigninErrorControllerFactory::GetForProfile(profile_)->HasError();
}
