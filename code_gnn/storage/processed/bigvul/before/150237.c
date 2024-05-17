gfx::Rect GetCenteredBounds(const gfx::Rect& bounds_in_parent,
                            wm::WindowState* state_object) {
  gfx::Rect work_area_in_parent =
      screen_util::GetDisplayWorkAreaBoundsInParent(state_object->window());
  work_area_in_parent.ClampToCenteredSize(bounds_in_parent.size());
  return work_area_in_parent;
}
