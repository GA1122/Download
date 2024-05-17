ChromeWebUIControllerFactory* ChromeWebUIControllerFactory::GetInstance() {
  return Singleton< ChromeWebUIControllerFactory, PossibleTestSingletonTraits<
      ChromeWebUIControllerFactory, TestChromeWebUIControllerFactory> >::get();
}
