void SkiaOutputSurfaceImpl::SetCapabilitiesForTesting(
    bool flipped_output_surface) {
  DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);
  DCHECK(impl_on_gpu_);
  capabilities_.flipped_output_surface = flipped_output_surface;
  auto callback =
      base::BindOnce(&SkiaOutputSurfaceImplOnGpu::SetCapabilitiesForTesting,
                     base::Unretained(impl_on_gpu_.get()), capabilities_);
  ScheduleGpuTask(std::move(callback), std::vector<gpu::SyncToken>());
}
