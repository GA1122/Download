void RenderViewHostImpl::ClearPowerSaveBlockers() {
  STLDeleteValues(&power_save_blockers_);
}
