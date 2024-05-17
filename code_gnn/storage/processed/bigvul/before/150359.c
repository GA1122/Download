ClientControlledShellSurface::DelegateFactoryCallback& GetFactoryForTesting() {
  using CallbackType = ClientControlledShellSurface::DelegateFactoryCallback;
  static base::NoDestructor<CallbackType> factory;
  return *factory;
}
