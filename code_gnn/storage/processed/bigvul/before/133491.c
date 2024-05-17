AwMessagePortService* AwMainDelegate::CreateAwMessagePortService() {
  return new AwMessagePortServiceImpl();
}
