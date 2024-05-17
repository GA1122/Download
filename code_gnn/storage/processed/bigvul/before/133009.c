void RenderWidgetHostViewAura::RemovingFromRootWindow() {
  aura::client::CursorClient* cursor_client =
      aura::client::GetCursorClient(window_->GetRootWindow());
  if (cursor_client)
    cursor_client->RemoveObserver(this);

  DetachFromInputMethod();

  event_filter_for_popup_exit_.reset();
  window_->GetDispatcher()->RemoveRootWindowObserver(this);
  host_->ParentChanged(0);
  ui::Compositor* compositor = GetCompositor();
  if (current_surface_.get()) {
    window_->layer()->SetShowPaintedContent();
  }
  RunOnCommitCallbacks();
  resize_lock_.reset();
  host_->WasResized();
  if (compositor && compositor->HasObserver(this))
    compositor->RemoveObserver(this);
}
