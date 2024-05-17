void RenderWidgetHostImpl::Destroy(bool also_delete) {
  DCHECK(!destroyed_);
  destroyed_ = true;

  NotificationService::current()->Notify(
      NOTIFICATION_RENDER_WIDGET_HOST_DESTROYED, Source<RenderWidgetHost>(this),
      NotificationService::NoDetails());

  if (view_) {
    view_->Destroy();
    view_.reset();
  }

  for (const auto& id : owned_bitmaps_)
    viz::ServerSharedBitmapManager::current()->ChildDeletedSharedBitmap(id);

  process_->GetSharedBitmapAllocationNotifier()->RemoveObserver(this);
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
