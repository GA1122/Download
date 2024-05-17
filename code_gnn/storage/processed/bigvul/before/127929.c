bool BrowserViewRenderer::CompositeHw() {
  CancelFallbackTick();

  ReturnResourceFromParent();
  UpdateMemoryPolicy();

  ParentCompositorDrawConstraints parent_draw_constraints =
      shared_renderer_state_.GetParentDrawConstraintsOnUI();
  gfx::Size surface_size(size_);
  gfx::Rect viewport(surface_size);
  gfx::Rect clip = viewport;
  gfx::Transform transform_for_tile_priority =
      parent_draw_constraints.transform;

  gfx::Rect viewport_rect_for_tile_priority;

  if (!offscreen_pre_raster_ && !parent_draw_constraints.is_layer) {
    viewport_rect_for_tile_priority = last_on_draw_global_visible_rect_;
  }

  scoped_ptr<cc::CompositorFrame> frame =
      compositor_->DemandDrawHw(surface_size,
                                gfx::Transform(),
                                viewport,
                                clip,
                                viewport_rect_for_tile_priority,
                                transform_for_tile_priority);
  if (!frame.get()) {
    TRACE_EVENT_INSTANT0("android_webview", "NoNewFrame",
                         TRACE_EVENT_SCOPE_THREAD);
    return shared_renderer_state_.HasFrameOnUI();
  }

  scoped_ptr<ChildFrame> child_frame = make_scoped_ptr(
      new ChildFrame(frame.Pass(), viewport_rect_for_tile_priority.IsEmpty(),
                     transform_for_tile_priority, offscreen_pre_raster_,
                     parent_draw_constraints.is_layer));

  ReturnUnusedResource(shared_renderer_state_.PassUncommittedFrameOnUI());
  shared_renderer_state_.SetCompositorFrameOnUI(child_frame.Pass());
  return true;
}
