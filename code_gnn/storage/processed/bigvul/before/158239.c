void RenderWidgetHostImpl::Destroy(bool also_delete) {
  DCHECK(!destroyed_);
  destroyed_ = true;

  for (auto& observer : observers_)
    observer.RenderWidgetHostDestroyed(this);
  NotificationService::current()->Notify(
      NOTIFICATION_RENDER_WIDGET_HOST_DESTROYED, Source<RenderWidgetHost>(this),
      NotificationService::NoDetails());

  if (view_) {
    view_->Destroy();
    view_.reset();
  }

  if (shared_bitmap_manager_) {
    for (const auto& id : owned_bitmaps_)
      shared_bitmap_manager_->ChildDeletedSharedBitmap(id);
  } else {
    DCHECK(owned_bitmaps_.empty());
  }

  process_->RemoveWidget(this);
  process_->RemoveRoute(routing_id_);
  g_routing_id_widget_map.Get().erase(
      RenderWidgetHostID(process_->GetID(), routing_id_));

  if (delegate_)
    delegate_->RenderWidgetDeleted(this);

  if (also_delete) {
    CHECK(!owner_delegate_);
    delete this;
  }
}
