SandboxPolicyFuchsia::~SandboxPolicyFuchsia() {
  if (service_directory_) {
    service_directory_task_runner_->DeleteSoon(FROM_HERE,
                                               std::move(service_directory_));
  }
}
