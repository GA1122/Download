std::unique_ptr<ui::CompositorLock> CompositorImpl::GetCompositorLock(
    ui::CompositorLockClient* client,
    base::TimeDelta timeout) {
  std::unique_ptr<cc::ScopedDeferMainFrameUpdate>
      scoped_defer_main_frame_update =
          host_ ? host_->DeferMainFrameUpdate() : nullptr;
  return lock_manager_.GetCompositorLock(
      client, timeout, std::move(scoped_defer_main_frame_update));
}
