void RenderWidgetHostImpl::ScrollBackingStoreRect(const gfx::Vector2d& delta,
                                                  const gfx::Rect& clip_rect,
                                                  const gfx::Size& view_size) {
  if (is_hidden_) {
    needs_repainting_on_restore_ = true;
    return;
  }

  BackingStore* backing_store = BackingStoreManager::Lookup(this);
  if (!backing_store || (backing_store->size() != view_size))
    return;
  backing_store->ScrollBackingStore(delta, clip_rect, view_size);
}
