GpuProcessHost::~GpuProcessHost() {
  DCHECK(CalledOnValidThread());

  SendOutstandingReplies();
  if (process_launched_ && kind_ == GPU_PROCESS_KIND_SANDBOXED) {
    if (software_rendering_) {
      UMA_HISTOGRAM_ENUMERATION("GPU.SoftwareRendererLifetimeEvents",
                                DIED_FIRST_TIME + g_gpu_software_crash_count,
                                GPU_PROCESS_LIFETIME_EVENT_MAX);

      if (++g_gpu_software_crash_count >= kGpuMaxCrashCount) {
        gpu_enabled_ = false;
      }
    } else {
      UMA_HISTOGRAM_ENUMERATION("GPU.GPUProcessLifetimeEvents",
                                DIED_FIRST_TIME + g_gpu_crash_count,
                                GPU_PROCESS_LIFETIME_EVENT_MAX);

      if (++g_gpu_crash_count >= kGpuMaxCrashCount) {
#if !defined(OS_CHROMEOS)
        hardware_gpu_enabled_ = false;
        GpuDataManagerImpl::GetInstance()->BlacklistCard();
#endif
      }
    }
  }

  int exit_code;
  base::TerminationStatus status = process_->GetTerminationStatus(&exit_code);
  UMA_HISTOGRAM_ENUMERATION("GPU.GPUProcessTerminationStatus",
                            status,
                            base::TERMINATION_STATUS_MAX_ENUM);

  UMA_HISTOGRAM_COUNTS_100("GPU.SurfaceCountAtExit",
                           GpuSurfaceTracker::Get()->GetSurfaceCount());

  if (status == base::TERMINATION_STATUS_NORMAL_TERMINATION ||
      status == base::TERMINATION_STATUS_ABNORMAL_TERMINATION) {
    UMA_HISTOGRAM_ENUMERATION("GPU.GPUProcessExitCode",
                              exit_code,
                              RESULT_CODE_LAST_CODE);
  }

  while (!queued_messages_.empty()) {
    delete queued_messages_.front();
    queued_messages_.pop();
  }

  if (g_gpu_process_hosts[kind_] == this)
    g_gpu_process_hosts[kind_] = NULL;

  BrowserThread::PostTask(BrowserThread::UI,
                          FROM_HERE,
                          base::Bind(&GpuProcessHostUIShim::Destroy, host_id_));
}
