void HTMLCanvasElement::FinalizeFrame() {
  TRACE_EVENT0("blink", "HTMLCanvasElement::FinalizeFrame");
  RecordCanvasSizeToUMA(size_,
                        CanvasRenderingContextHost::HostType::kCanvasHost);

  auto* resource_provider = ResourceProvider();
  if (resource_provider)
    resource_provider->ReleaseLockedImages();

  if (canvas2d_bridge_) {
    if (IsAccelerated() &&
        canvas_heuristic_parameters::kGPUReadbackForcesNoAcceleration &&
        !RuntimeEnabledFeatures::Canvas2dFixedRenderingModeEnabled() &&
        !base::FeatureList::IsEnabled(features::kAlwaysAccelerateCanvas)) {
      if (gpu_readback_invoked_in_current_frame_) {
        gpu_readback_successive_frames_++;
        gpu_readback_invoked_in_current_frame_ = false;
      } else {
        gpu_readback_successive_frames_ = 0;
      }

      if (gpu_readback_successive_frames_ >=
          canvas_heuristic_parameters::kGPUReadbackMinSuccessiveFrames) {
        DisableAcceleration();
      }
    }

    if (!LowLatencyEnabled())
      canvas2d_bridge_->FinalizeFrame();
  }

  if (LowLatencyEnabled() && !dirty_rect_.IsEmpty()) {
    if (GetOrCreateCanvasResourceProvider(kPreferAcceleration)) {
      if (!ResourceProvider()->IsSingleBuffered()) {
        ResourceProvider()->TryEnableSingleBuffering();
        if (Is3d() && RuntimeEnabledFeatures::WebGLImageChromiumEnabled())
          context_->ProvideBackBufferToResourceProvider();
      }

      if (canvas2d_bridge_) {
        canvas2d_bridge_->FlushRecording();
      } else {
        DCHECK(Is3d());
        if (!RuntimeEnabledFeatures::WebGLImageChromiumEnabled())
          context_->PaintRenderingResultsToCanvas(kBackBuffer);
      }

      const base::TimeTicks start_time = WTF::CurrentTimeTicks();
      const scoped_refptr<CanvasResource> canvas_resource =
          ResourceProvider()->ProduceCanvasResource();
      const FloatRect src_rect(0, 0, Size().Width(), Size().Height());
      dirty_rect_.Intersect(src_rect);
      const IntRect int_dirty = EnclosingIntRect(dirty_rect_);
      const SkIRect damage_rect = SkIRect::MakeXYWH(
          int_dirty.X(), int_dirty.Y(), int_dirty.Width(), int_dirty.Height());
      const bool needs_vertical_flip = !RenderingContext()->IsOriginTopLeft();
      frame_dispatcher_->DispatchFrame(std::move(canvas_resource), start_time,
                                       damage_rect, needs_vertical_flip,
                                       IsOpaque());
      dirty_rect_ = FloatRect();
    }
  }

  if (!did_notify_listeners_for_current_frame_)
    NotifyListenersCanvasChanged();
  did_notify_listeners_for_current_frame_ = false;
}
