  void OnVisibilityChanged() {
    if (visible_compositors_.size() > 0) {
      GpuDataManagerImpl::GetInstance()->SetApplicationVisible(true);
      BrowserGpuChannelHostFactorySetApplicationVisible(true);
      SendOnForegroundedToGpuService();
      low_end_background_cleanup_task_.Cancel();
    } else {
      GpuDataManagerImpl::GetInstance()->SetApplicationVisible(false);
      BrowserGpuChannelHostFactorySetApplicationVisible(false);
      SendOnBackgroundedToGpuService();
      EnqueueLowEndBackgroundCleanup();
    }
  }
