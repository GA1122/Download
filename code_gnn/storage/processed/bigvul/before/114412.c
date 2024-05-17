 GpuCommandBufferStub::SurfaceState::SurfaceState(int32 surface_id,
                                                  bool visible,
                                                  base::TimeTicks last_used_time)
    : surface_id(surface_id),
      visible(visible),
      last_used_time(last_used_time) {
}
