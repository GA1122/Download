void ShellContentUtilityClient::UtilityThreadStarted() {
  auto registry = std::make_unique<service_manager::BinderRegistry>();
  registry->AddInterface(base::BindRepeating(&TestServiceImpl::Create),
                         base::ThreadTaskRunnerHandle::Get());
  registry->AddInterface<mojom::PowerMonitorTest>(
      base::BindRepeating(
          &PowerMonitorTestImpl::MakeStrongBinding,
          base::Passed(std::make_unique<PowerMonitorTestImpl>())),
      base::ThreadTaskRunnerHandle::Get());
  content::ChildThread::Get()
      ->GetServiceManagerConnection()
      ->AddConnectionFilter(
          std::make_unique<SimpleConnectionFilter>(std::move(registry)));
}
