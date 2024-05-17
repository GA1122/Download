void RenderWidgetHostImpl::DidDeleteSharedBitmap(
    const viz::SharedBitmapId& id) {
  viz::ServerSharedBitmapManager::current()->ChildDeletedSharedBitmap(id);
  owned_bitmaps_.erase(id);
}
