void ApiTestBase::RunTest(const std::string& file_name,
                          const std::string& test_name) {
  ExpectNoAssertionsMade();
  test_env_->RunTest(file_name, test_name);
}
