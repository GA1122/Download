bool TracingControllerImpl::GetTraceBufferUsage(
    GetTraceBufferUsageCallback callback) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);

  ConnectToServiceIfNeeded();
  coordinator_->RequestBufferUsage(base::BindOnce(
      [](GetTraceBufferUsageCallback callback, bool success, float percent_full,
         uint32_t approximate_count) {
        std::move(callback).Run(percent_full, approximate_count);
      },
      std::move(callback)));
  return true;
}
