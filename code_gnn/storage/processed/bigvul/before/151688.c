bool Browser::HasCompletedUnloadProcessing() const {
  DCHECK(IsFastTabUnloadEnabled());
  return fast_unload_controller_->HasCompletedUnloadProcessing();
}
