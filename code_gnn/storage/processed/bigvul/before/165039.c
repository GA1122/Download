bool HTMLCanvasElement::ShouldAccelerate(AccelerationCriteria criteria) const {
  if (!base::FeatureList::IsEnabled(features::kAlwaysAccelerateCanvas)) {
    if (context_ && !Is2d())
      return false;

    if (GetLayoutBox() && !GetLayoutBox()->HasAcceleratedCompositing())
      return false;

    base::CheckedNumeric<int> checked_canvas_pixel_count = Size().Width();
    checked_canvas_pixel_count *= Size().Height();
    if (!checked_canvas_pixel_count.IsValid())
      return false;
    int canvas_pixel_count = checked_canvas_pixel_count.ValueOrDie();

    if (criteria != kIgnoreResourceLimitCriteria) {
      Settings* settings = GetDocument().GetSettings();
      if (!settings ||
          canvas_pixel_count < settings->GetMinimumAccelerated2dCanvasSize()) {
        return false;
      }

      if (global_gpu_memory_usage_ >= kMaxGlobalGPUMemoryUsage)
        return false;

      if (global_accelerated_context_count_ >=
          kMaxGlobalAcceleratedResourceCount)
        return false;
    }
  }

  base::WeakPtr<WebGraphicsContext3DProviderWrapper> context_provider_wrapper =
      SharedGpuContext::ContextProviderWrapper();
  if (!context_provider_wrapper)
    return false;

  return context_provider_wrapper->Utils()->Accelerated2DCanvasFeatureEnabled();
}
