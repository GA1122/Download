void SandboxPolicyFuchsia::UpdateLaunchOptionsForSandbox(
    base::LaunchOptions* options) {
  DCHECK_NE(type_, service_manager::SANDBOX_TYPE_INVALID);

  options->fds_to_remap.push_back(std::make_pair(STDERR_FILENO, STDERR_FILENO));

  if (type_ == service_manager::SANDBOX_TYPE_NO_SANDBOX) {
    options->spawn_flags = FDIO_SPAWN_CLONE_NAMESPACE | FDIO_SPAWN_CLONE_JOB;
    options->clear_environ = false;
    return;
  }

  options->paths_to_clone.push_back(base::GetPackageRoot());

  options->clear_environ = true;

  options->spawn_flags = 0;

  if (service_directory_) {
    options->paths_to_transfer.push_back(base::PathToTransfer{
        base::FilePath("/svc"), service_directory_->ConnectClient().release()});
  }
}
