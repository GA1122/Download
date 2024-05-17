gpu::SyncToken SkiaOutputSurfaceImpl::SubmitPaint(
    base::OnceClosure on_finished) {
  DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);
  DCHECK(current_paint_);
  DCHECK(!deferred_framebuffer_draw_closure_);

  bool painting_render_pass = current_paint_->render_pass_id() != 0;

  gpu::SyncToken sync_token(
      gpu::CommandBufferNamespace::VIZ_SKIA_OUTPUT_SURFACE,
      impl_on_gpu_->command_buffer_id(), ++sync_fence_release_);
  sync_token.SetVerifyFlush();

  auto ddl = current_paint_->recorder()->detach();
  DCHECK(ddl);
  std::unique_ptr<SkDeferredDisplayList> overdraw_ddl;
  if (renderer_settings_.show_overdraw_feedback && !painting_render_pass) {
    overdraw_ddl = overdraw_surface_recorder_->detach();
    DCHECK(overdraw_ddl);
    overdraw_canvas_.reset();
    nway_canvas_.reset();
    overdraw_surface_recorder_.reset();
  }

  if (painting_render_pass) {
    auto it = render_pass_image_cache_.find(current_paint_->render_pass_id());
    if (it != render_pass_image_cache_.end()) {
      it->second->clear_image();
    }
    DCHECK(!on_finished);
    auto closure = base::BindOnce(
        &SkiaOutputSurfaceImplOnGpu::FinishPaintRenderPass,
        base::Unretained(impl_on_gpu_.get()), current_paint_->render_pass_id(),
        std::move(ddl), std::move(images_in_current_paint_),
        resource_sync_tokens_, sync_fence_release_);
    ScheduleGpuTask(std::move(closure), std::move(resource_sync_tokens_));
  } else {
    deferred_framebuffer_draw_closure_ = base::BindOnce(
        &SkiaOutputSurfaceImplOnGpu::FinishPaintCurrentFrame,
        base::Unretained(impl_on_gpu_.get()), std::move(ddl),
        std::move(overdraw_ddl), std::move(images_in_current_paint_),
        resource_sync_tokens_, sync_fence_release_, std::move(on_finished),
        draw_rectangle_);
    draw_rectangle_.reset();
  }
  images_in_current_paint_.clear();
  current_paint_.reset();
  return sync_token;
}
