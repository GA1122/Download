void SkiaOutputSurfaceImpl::RemoveRenderPassResource(
    std::vector<RenderPassId> ids) {
  DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);
  DCHECK(!ids.empty());

  std::vector<std::unique_ptr<ImageContextImpl>> image_contexts;
  image_contexts.reserve(ids.size());
  for (const auto id : ids) {
    auto it = render_pass_image_cache_.find(id);
    if (it != render_pass_image_cache_.end()) {
      it->second->clear_image();
      image_contexts.push_back(std::move(it->second));
      render_pass_image_cache_.erase(it);
    }
  }

  if (!image_contexts.empty()) {
    auto callback = base::BindOnce(
        &SkiaOutputSurfaceImplOnGpu::RemoveRenderPassResource,
        base::Unretained(impl_on_gpu_.get()), std::move(image_contexts));
    ScheduleGpuTask(std::move(callback), std::vector<gpu::SyncToken>());
  }
}
