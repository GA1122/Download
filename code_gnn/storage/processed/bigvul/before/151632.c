std::string ChromePaymentRequestDelegate::GetAuthenticatedEmail() const {
  Profile* profile =
      Profile::FromBrowserContext(web_contents_->GetBrowserContext());
  SigninManagerBase* signin_manager =
      SigninManagerFactory::GetForProfile(profile);
  if (signin_manager && signin_manager->IsAuthenticated())
    return signin_manager->GetAuthenticatedAccountInfo().email;

  return std::string();
}
