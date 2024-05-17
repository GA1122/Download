SkCanvas* SkiaOutputSurfaceImpl::BeginPaintRenderPass(
    const RenderPassId& id,
    const gfx::Size& surface_size,
    ResourceFormat format,
    bool mipmap,
    sk_sp<SkColorSpace> color_space) {
  DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);
  DCHECK(!current_paint_);
  DCHECK(resource_sync_tokens_.empty());

  SkSurfaceCharacterization c = CreateSkSurfaceCharacterization(
      surface_size, format, mipmap, std::move(color_space));
  current_paint_.emplace(c, id);
  return current_paint_->recorder()->getCanvas();
}
