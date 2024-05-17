void ApiTestBase::SetUp() {
  ModuleSystemTest::SetUp();
  test_env_.reset(new ApiTestEnvironment(env()));
}
