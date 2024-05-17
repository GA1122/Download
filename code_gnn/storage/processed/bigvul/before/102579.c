void NaClProcessHost::OnProcessLaunchedByBroker(base::ProcessHandle handle) {
  set_handle(handle);
  OnProcessLaunched();
}
