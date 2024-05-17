void RenderWidgetHostViewAura::SubmitCompositorFrame(
    const cc::LocalSurfaceId& local_surface_id,
    cc::CompositorFrame frame) {
  TRACE_EVENT0("content", "RenderWidgetHostViewAura::OnSwapCompositorFrame");

  UpdateBackgroundColorFromRenderer(frame.metadata.root_background_color);

  last_scroll_offset_ = frame.metadata.root_scroll_offset;

  cc::Selection<gfx::SelectionBound> selection = frame.metadata.selection;
  if (IsUseZoomForDSFEnabled()) {
    float viewportToDIPScale = 1.0f / current_device_scale_factor_;
    gfx::PointF start_edge_top = selection.start.edge_top();
    gfx::PointF start_edge_bottom = selection.start.edge_bottom();
    gfx::PointF end_edge_top = selection.end.edge_top();
    gfx::PointF end_edge_bottom = selection.end.edge_bottom();

    start_edge_top.Scale(viewportToDIPScale);
    start_edge_bottom.Scale(viewportToDIPScale);
    end_edge_top.Scale(viewportToDIPScale);
    end_edge_bottom.Scale(viewportToDIPScale);

    selection.start.SetEdge(start_edge_top, start_edge_bottom);
    selection.end.SetEdge(end_edge_top, end_edge_bottom);
  }

  if (delegated_frame_host_) {
    delegated_frame_host_->SubmitCompositorFrame(local_surface_id,
                                                 std::move(frame));
  }
  selection_controller_->OnSelectionBoundsChanged(selection.start,
                                                  selection.end);
}
