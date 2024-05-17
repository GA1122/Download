void GpuProcessHost::OnProcessLaunched() {
  UMA_HISTOGRAM_TIMES("GPU.GPUProcessLaunchTime",
                      base::TimeTicks::Now() - init_start_time_);
#if defined(OS_WIN)
  if (kind_ == GPU_PROCESS_KIND_SANDBOXED)
    RecordAppContainerStatus(sandbox::SBOX_ALL_OK, crashed_before_);
#endif   
}
