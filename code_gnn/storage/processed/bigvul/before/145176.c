void RecordAppContainerStatus(int error_code, bool crashed_before) {
  base::CommandLine* command_line = base::CommandLine::ForCurrentProcess();
  if (!crashed_before &&
      service_manager::SandboxWin::IsAppContainerEnabledForSandbox(
          *command_line, service_manager::SANDBOX_TYPE_GPU)) {
    base::UmaHistogramSparse("GPU.AppContainer.Status", error_code);
  }
}
