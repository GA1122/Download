bool Browser::TabsNeedBeforeUnloadFired() {
  if (IsFastTabUnloadEnabled())
    return fast_unload_controller_->TabsNeedBeforeUnloadFired();
  return unload_controller_->TabsNeedBeforeUnloadFired();
}
