gpu::SyncToken SkiaOutputSurfaceImpl::SkiaSwapBuffers(OutputSurfaceFrame frame,
                                                      bool wants_sync_token) {
  DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);
  DCHECK(!current_paint_);

  gpu::SyncToken sync_token;
  if (wants_sync_token) {
    sync_token = gpu::SyncToken(
        gpu::CommandBufferNamespace::VIZ_SKIA_OUTPUT_SURFACE,
        impl_on_gpu_->command_buffer_id(), ++sync_fence_release_);
    sync_token.SetVerifyFlush();
  }

  auto callback =
      base::BindOnce(&SkiaOutputSurfaceImplOnGpu::SwapBuffers,
                     base::Unretained(impl_on_gpu_.get()), std::move(frame),
                     std::move(deferred_framebuffer_draw_closure_),
                     sync_token.release_count());
  ScheduleGpuTask(std::move(callback), std::move(resource_sync_tokens_));

  RecreateRootRecorder();

  return sync_token;
}
