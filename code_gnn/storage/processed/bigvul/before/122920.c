void RenderProcessHostImpl::SurfaceUpdated(int32 surface_id) {
  if (!gpu_message_filter_)
    return;
  BrowserThread::PostTask(BrowserThread::IO, FROM_HERE, base::Bind(
      &GpuMessageFilter::SurfaceUpdated,
      gpu_message_filter_,
      surface_id));
}
