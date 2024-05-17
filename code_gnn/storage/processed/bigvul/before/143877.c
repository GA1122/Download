void SandboxPolicyFuchsia::Initialize(service_manager::SandboxType type) {
  DCHECK_NE(type, service_manager::SANDBOX_TYPE_INVALID);
  DCHECK_EQ(type_, service_manager::SANDBOX_TYPE_INVALID);

  type_ = type;

  if (base::CommandLine::ForCurrentProcess()->HasSwitch(
          service_manager::switches::kNoSandbox)) {
    type_ = service_manager::SANDBOX_TYPE_NO_SANDBOX;
  }

  if (type_ == service_manager::SANDBOX_TYPE_RENDERER) {
    service_directory_task_runner_ = base::ThreadTaskRunnerHandle::Get();
    service_directory_ =
        std::make_unique<base::fuchsia::FilteredServiceDirectory>(
            base::fuchsia::ComponentContext::GetDefault());
    for (const char* service_name : kRendererServices)
      service_directory_->AddService(service_name);
  }
}
