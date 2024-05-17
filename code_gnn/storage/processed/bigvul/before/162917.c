 MemoryInstrumentation::GetCoordinatorBindingForCurrentThread() {
   mojom::CoordinatorPtr* coordinator =
      reinterpret_cast<mojom::CoordinatorPtr*>(tls_coordinator_.Get());
  if (!coordinator) {
    coordinator = new mojom::CoordinatorPtr();
    tls_coordinator_.Set(coordinator);
    mojom::CoordinatorRequest coordinator_req = mojo::MakeRequest(coordinator);

    connector_task_runner_->PostTask(
        FROM_HERE,
        base::Bind(
            &MemoryInstrumentation::BindCoordinatorRequestOnConnectorThread,
            base::Unretained(this), base::Passed(std::move(coordinator_req))));
  }
  DCHECK(*coordinator);
  return *coordinator;
}
