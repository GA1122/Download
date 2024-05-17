GpuProcessHost::GpuProcessHost(int host_id, GpuProcessKind kind)
    : host_id_(host_id),
      valid_(true),
      in_process_(false),
      kind_(kind),
      process_launched_(false),
      status_(UNKNOWN),
#if defined(OS_MACOSX)
      ca_transaction_gpu_coordinator_(
          base::MakeRefCounted<CATransactionGPUCoordinator>(this)),
#endif
      gpu_host_binding_(this),
      weak_ptr_factory_(this) {
  if (base::CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kSingleProcess) ||
      base::CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kInProcessGPU)) {
    in_process_ = true;
  }

  DCHECK(!in_process_ || g_gpu_process_hosts[kind] == nullptr);

  g_gpu_process_hosts[kind] = this;

  process_.reset(new BrowserChildProcessHostImpl(
      PROCESS_TYPE_GPU, this, mojom::kGpuServiceName));
}
