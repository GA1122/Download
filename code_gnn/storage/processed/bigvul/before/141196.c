void Document::RemoveActivationBlockingDisplayLock() {
  DCHECK_GT(activation_blocking_display_lock_count_, 0);
  --activation_blocking_display_lock_count_;
}
