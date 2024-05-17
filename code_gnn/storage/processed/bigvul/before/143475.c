  AndroidOutputSurface(
      scoped_refptr<ws::ContextProviderCommandBuffer> context_provider,
      base::RepeatingCallback<void(const gfx::Size&)> swap_buffers_callback)
      : viz::OutputSurface(std::move(context_provider)),
        swap_buffers_callback_(std::move(swap_buffers_callback)),
        overlay_candidate_validator_(
            new viz::CompositorOverlayCandidateValidatorAndroid()),
        weak_ptr_factory_(this) {
    capabilities_.max_frames_pending = kMaxDisplaySwapBuffers;
  }
