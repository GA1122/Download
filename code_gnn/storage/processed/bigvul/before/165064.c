CanvasResourceProvider* OffscreenCanvas::GetOrCreateResourceProvider() {
  if (!ResourceProvider()) {
    bool can_use_gpu = false;
    CanvasResourceProvider::PresentationMode presentation_mode =
        CanvasResourceProvider::kDefaultPresentationMode;
    if (Is2d()) {
      if (RuntimeEnabledFeatures::Canvas2dImageChromiumEnabled()) {
        presentation_mode =
            CanvasResourceProvider::kAllowImageChromiumPresentationMode;
      }
      if (SharedGpuContext::IsGpuCompositingEnabled() &&
          RuntimeEnabledFeatures::Accelerated2dCanvasEnabled()) {
        can_use_gpu = true;
      }
    } else if (Is3d()) {
      if (RuntimeEnabledFeatures::WebGLImageChromiumEnabled()) {
        presentation_mode =
            CanvasResourceProvider::kAllowImageChromiumPresentationMode;
      }
      can_use_gpu = SharedGpuContext::IsGpuCompositingEnabled();
    }

    IntSize surface_size(width(), height());
    CanvasResourceProvider::ResourceUsage usage;
    if (can_use_gpu) {
      if (HasPlaceholderCanvas())
        usage = CanvasResourceProvider::kAcceleratedCompositedResourceUsage;
      else
        usage = CanvasResourceProvider::kAcceleratedResourceUsage;
    } else {
      if (HasPlaceholderCanvas())
        usage = CanvasResourceProvider::kSoftwareCompositedResourceUsage;
      else
        usage = CanvasResourceProvider::kSoftwareResourceUsage;
    }

    base::WeakPtr<CanvasResourceDispatcher> dispatcher_weakptr =
        HasPlaceholderCanvas() ? GetOrCreateResourceDispatcher()->GetWeakPtr()
                               : nullptr;

    ReplaceResourceProvider(CanvasResourceProvider::Create(
        surface_size, usage, SharedGpuContext::ContextProviderWrapper(), 0,
        context_->ColorParams(), presentation_mode,
        std::move(dispatcher_weakptr), false  ));

    CHECK(!ResourceProvider() || !HasPlaceholderCanvas() ||
          ResourceProvider()->SupportsDirectCompositing());

    if (ResourceProvider() && ResourceProvider()->IsValid()) {
      ResourceProvider()->Clear();
      ResourceProvider()->Canvas()->save();

      if (needs_matrix_clip_restore_) {
        needs_matrix_clip_restore_ = false;
        context_->RestoreCanvasMatrixClipStack(ResourceProvider()->Canvas());
      }
    }
  }
  return ResourceProvider();
}
