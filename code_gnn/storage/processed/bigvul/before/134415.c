void TabStrip::ScheduleRemoveTabAnimation(Tab* tab) {
  GenerateIdealBounds();
  AnimateToIdealBounds();

  gfx::Rect tab_bounds = tab->bounds();
  tab_bounds.set_width(0);
  bounds_animator_.AnimateViewTo(tab, tab_bounds);

  bounds_animator_.SetAnimationDelegate(tab, new RemoveTabDelegate(this, tab),
                                        true);

  if (TabDragController::IsAttachedTo(this)) {
    bounds_animator_.StopAnimatingView(newtab_button_);
    newtab_button_->SetBoundsRect(newtab_button_bounds_);
  }
}
