  void EnqueueLowEndBackgroundCleanup() {
    if (base::SysInfo::IsLowEndDevice()) {
      low_end_background_cleanup_task_.Reset(
          base::BindOnce(&CompositorDependencies::DoLowEndBackgroundCleanup,
                         base::Unretained(this)));
      base::ThreadTaskRunnerHandle::Get()->PostDelayedTask(
          FROM_HERE, low_end_background_cleanup_task_.callback(),
          base::TimeDelta::FromSeconds(5));
    }
  }
