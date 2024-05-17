void TabStrip::StartMouseInitiatedRemoveTabAnimation(int model_index) {
  Tab* tab_closing = tab_at(model_index);
  int delta = tab_closing->width() + tab_h_offset();
  DCHECK_NE(model_index + 1, tab_count());
  if (tab_closing->data().mini && model_index + 1 < tab_count() &&
      !tab_at(model_index + 1)->data().mini) {
    delta += kMiniToNonMiniGap;
  }

  for (int i = model_index + 1; i < tab_count(); ++i) {
    gfx::Rect bounds = ideal_bounds(i);
    bounds.set_x(bounds.x() - delta);
    set_ideal_bounds(i, bounds);
  }

  newtab_button_bounds_.set_x(newtab_button_bounds_.x() - delta);

  PrepareForAnimation();

  tab_closing->set_closing(true);

  RemoveTabFromViewModel(model_index);

  AnimateToIdealBounds();

  gfx::Rect tab_bounds = tab_closing->bounds();
  tab_bounds.set_width(0);
  bounds_animator_.AnimateViewTo(tab_closing, tab_bounds);

  bounds_animator_.SetAnimationDelegate(
      tab_closing,
      new RemoveTabDelegate(this, tab_closing),
      true);
}
