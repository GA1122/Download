void GpuProcessHost::OnProcessLaunchFailed(int error_code) {
  int process_launch_error_code = error_code;
  base::debug::Alias(&process_launch_error_code);

#if defined(OS_WIN)
  if (kind_ == GPU_PROCESS_KIND_SANDBOXED)
    RecordAppContainerStatus(error_code, crashed_before_);
#endif   
  RecordProcessCrash();
}
