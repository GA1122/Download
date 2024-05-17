GpuDataManager::GpuDataManager()
    : complete_gpu_info_already_requested_(false) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  GPUInfo gpu_info;
  gpu_info_collector::CollectPreliminaryGraphicsInfo(&gpu_info);
  UpdateGpuInfo(gpu_info);
}
