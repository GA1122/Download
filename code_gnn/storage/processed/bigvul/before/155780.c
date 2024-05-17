  DiceResponseHandlerFactory()
      : BrowserContextKeyedServiceFactory(
            "DiceResponseHandler",
            BrowserContextDependencyManager::GetInstance()) {
    DependsOn(AboutSigninInternalsFactory::GetInstance());
    DependsOn(AccountReconcilorFactory::GetInstance());
    DependsOn(AccountTrackerServiceFactory::GetInstance());
    DependsOn(ChromeSigninClientFactory::GetInstance());
    DependsOn(ProfileOAuth2TokenServiceFactory::GetInstance());
    DependsOn(SigninManagerFactory::GetInstance());
  }
