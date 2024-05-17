void RenderWidgetHostImpl::WasShown() {
  if (!is_hidden_)
    return;
  is_hidden_ = false;

  SendScreenRects();

  BackingStore* backing_store = BackingStoreManager::Lookup(this);
  bool needs_repainting;
  if (needs_repainting_on_restore_ || !backing_store ||
      is_accelerated_compositing_active()) {
    needs_repainting = true;
    needs_repainting_on_restore_ = false;
  } else {
    needs_repainting = false;
  }
  Send(new ViewMsg_WasShown(routing_id_, needs_repainting));

  process_->WidgetRestored();

  bool is_visible = true;
  NotificationService::current()->Notify(
      NOTIFICATION_RENDER_WIDGET_VISIBILITY_CHANGED,
      Source<RenderWidgetHost>(this),
      Details<bool>(&is_visible));

  WasResized();
}
