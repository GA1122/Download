void ComponentUpdaterPolicyTest::EndTest() {
  post_interceptor_.reset();
  cus_ = nullptr;

  base::RunLoop::QuitCurrentWhenIdleDeprecated();
}
