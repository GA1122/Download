TestRunner::~TestRunner() {
  if (policy_)
    policy_->Release();
}
