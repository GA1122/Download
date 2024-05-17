SynchronousCompositorOutputSurface::SynchronousCompositorOutputSurface(
    const scoped_refptr<cc::ContextProvider>& context_provider,
    const scoped_refptr<cc::ContextProvider>& worker_context_provider,
    int routing_id,
    SynchronousCompositorRegistry* registry,
    scoped_refptr<FrameSwapMessageQueue> frame_swap_message_queue)
    : cc::OutputSurface(
          context_provider,
          worker_context_provider,
          scoped_ptr<cc::SoftwareOutputDevice>(new SoftwareDevice(this))),
      routing_id_(routing_id),
      registry_(registry),
      registered_(false),
      sync_client_(nullptr),
      next_hardware_draw_needs_damage_(false),
      current_sw_canvas_(nullptr),
      memory_policy_(0u),
      frame_swap_message_queue_(frame_swap_message_queue) {
  thread_checker_.DetachFromThread();
  DCHECK(registry_);
  capabilities_.adjust_deadline_for_parent = false;
  capabilities_.delegated_rendering = true;
  capabilities_.max_frames_pending = 1;
  memory_policy_.priority_cutoff_when_visible =
      gpu::MemoryAllocation::CUTOFF_ALLOW_NICE_TO_HAVE;
}
