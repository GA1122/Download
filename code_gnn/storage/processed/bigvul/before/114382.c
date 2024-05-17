GpuCommandBufferStub::GpuCommandBufferStub(
    GpuChannel* channel,
    GpuCommandBufferStub* share_group,
    const gfx::GLSurfaceHandle& handle,
    const gfx::Size& size,
    const gpu::gles2::DisallowedFeatures& disallowed_features,
    const std::string& allowed_extensions,
    const std::vector<int32>& attribs,
    gfx::GpuPreference gpu_preference,
    int32 route_id,
    int32 surface_id,
    GpuWatchdog* watchdog,
    bool software)
    : channel_(channel),
      handle_(handle),
      initial_size_(size),
      disallowed_features_(disallowed_features),
      allowed_extensions_(allowed_extensions),
      requested_attribs_(attribs),
      gpu_preference_(gpu_preference),
      route_id_(route_id),
      software_(software),
      last_flush_count_(0),
      allocation_(GpuMemoryAllocation::INVALID_RESOURCE_SIZE, true, true),
      parent_stub_for_initialization_(),
      parent_texture_for_initialization_(0),
      watchdog_(watchdog) {
  if (share_group) {
    context_group_ = share_group->context_group_;
  } else {
    context_group_ = new gpu::gles2::ContextGroup(true);
  }
  if (surface_id != 0)
    surface_state_.reset(new GpuCommandBufferStubBase::SurfaceState(
        surface_id, true, base::TimeTicks::Now()));
}
