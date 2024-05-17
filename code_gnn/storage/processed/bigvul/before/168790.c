bool RenderWidgetHostImpl::SurfacePropertiesMismatch(
    const RenderWidgetSurfaceProperties& first,
    const RenderWidgetSurfaceProperties& second) const {
#ifdef OS_ANDROID
  if (enable_surface_synchronization_) {
    RenderWidgetSurfaceProperties second_reduced = second;
    second_reduced.top_controls_height = first.top_controls_height;
    second_reduced.top_controls_shown_ratio = first.top_controls_shown_ratio;
    second_reduced.bottom_controls_height = first.bottom_controls_height;
    second_reduced.bottom_controls_shown_ratio =
        first.bottom_controls_shown_ratio;
    second_reduced.selection = first.selection;

    return first != second_reduced;
  }
#endif

  return first != second;
}
