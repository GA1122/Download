void SkiaOutputSurfaceImpl::SetEnableDCLayers(bool enable) {
  auto task = base::BindOnce(&SkiaOutputSurfaceImplOnGpu::SetEnableDCLayers,
                             base::Unretained(impl_on_gpu_.get()), enable);
  ScheduleGpuTask(std::move(task), {});
}
