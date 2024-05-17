std::unique_ptr<service_manager::Service> CreateTestService() {
  return std::unique_ptr<service_manager::Service>(new TestService);
}
