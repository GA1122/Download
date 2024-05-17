GpuProcessHost::~GpuProcessHost() {
  DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);
  if (in_process_gpu_thread_)
    DCHECK(process_);

  SendOutstandingReplies(EstablishChannelStatus::GPU_HOST_INVALID);

#if defined(OS_MACOSX)
  ca_transaction_gpu_coordinator_->HostWillBeDestroyed();
  ca_transaction_gpu_coordinator_ = nullptr;
#endif

  if (status_ == UNKNOWN) {
    RunRequestGPUInfoCallbacks(gpu::GPUInfo());
  } else {
    DCHECK(request_gpu_info_callbacks_.empty());
  }

  while (!queued_messages_.empty()) {
    delete queued_messages_.front();
    queued_messages_.pop();
  }

  if (g_gpu_process_hosts[kind_] == this)
    g_gpu_process_hosts[kind_] = nullptr;

#if defined(OS_ANDROID)
  UMA_HISTOGRAM_COUNTS_100("GPU.AtExitSurfaceCount",
                           gpu::GpuSurfaceTracker::Get()->GetSurfaceCount());
#endif

  std::string message;
  bool block_offscreen_contexts = true;
  if (!in_process_ && process_launched_) {
    ChildProcessTerminationInfo info =
        process_->GetTerminationInfo(false  );
    UMA_HISTOGRAM_ENUMERATION("GPU.GPUProcessTerminationStatus2",
                              ConvertToGpuTerminationStatus(info.status),
                              GpuTerminationStatus::MAX_ENUM);

    if (info.status == base::TERMINATION_STATUS_NORMAL_TERMINATION ||
        info.status == base::TERMINATION_STATUS_ABNORMAL_TERMINATION ||
        info.status == base::TERMINATION_STATUS_PROCESS_CRASHED) {
      base::UmaHistogramSparse("GPU.GPUProcessExitCode",
                               std::max(0, std::min(100, info.exit_code)));
    }

    switch (info.status) {
      case base::TERMINATION_STATUS_NORMAL_TERMINATION:
#if defined(OS_ANDROID)
        block_offscreen_contexts = false;
#endif
        message = "The GPU process exited normally. Everything is okay.";
        break;
      case base::TERMINATION_STATUS_ABNORMAL_TERMINATION:
        message = base::StringPrintf("The GPU process exited with code %d.",
                                     info.exit_code);
        break;
      case base::TERMINATION_STATUS_PROCESS_WAS_KILLED:
        message = "You killed the GPU process! Why?";
        break;
#if defined(OS_CHROMEOS)
      case base::TERMINATION_STATUS_PROCESS_WAS_KILLED_BY_OOM:
        message = "The GUP process was killed due to out of memory.";
        break;
#endif
      case base::TERMINATION_STATUS_PROCESS_CRASHED:
        message = "The GPU process crashed!";
        break;
      case base::TERMINATION_STATUS_LAUNCH_FAILED:
        message = "The GPU process failed to start!";
        break;
      default:
        break;
    }
  }

  if (block_offscreen_contexts)
    BlockLiveOffscreenContexts();

  BrowserThread::PostTask(
      BrowserThread::UI, FROM_HERE,
      base::BindOnce(&OnGpuProcessHostDestroyedOnUI, host_id_, message));
}
