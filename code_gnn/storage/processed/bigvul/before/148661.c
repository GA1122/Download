void SkiaOutputSurfaceImpl::ScheduleDCLayers(
    std::vector<DCLayerOverlay> overlays) {
  auto task =
      base::BindOnce(&SkiaOutputSurfaceImplOnGpu::ScheduleDCLayers,
                     base::Unretained(impl_on_gpu_.get()), std::move(overlays));
  ScheduleGpuTask(std::move(task), {});
}
