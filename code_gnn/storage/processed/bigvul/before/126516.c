TabGtk* TabStripGtk::GetTabAtAdjustForAnimation(int index) const {
  if (active_animation_.get() &&
      active_animation_->type() == TabAnimation::REMOVE &&
      index >=
      static_cast<RemoveTabAnimation*>(active_animation_.get())->index()) {
    index++;
  }
  return GetTabAt(index);
}
