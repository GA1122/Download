  scoped_ptr<cc::CompositorFrame> TestSynchronousCompositor::DemandDrawHw(
    gfx::Size surface_size,
//     const gfx::Size& surface_size,
      const gfx::Transform& transform,
    gfx::Rect viewport,
    gfx::Rect clip,
    gfx::Rect viewport_rect_for_tile_priority,
//     const gfx::Rect& viewport,
//     const gfx::Rect& clip,
//     const gfx::Rect& viewport_rect_for_tile_priority,
      const gfx::Transform& transform_for_tile_priority) {
    return hardware_frame_.Pass();
  }