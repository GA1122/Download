RenderWidgetHostViewAura::~RenderWidgetHostViewAura() {
  if (paint_observer_)
    paint_observer_->OnViewDestroyed();
  if (touch_editing_client_)
    touch_editing_client_->OnViewDestroyed();

  ImageTransportFactory::GetInstance()->RemoveObserver(this);

  window_observer_.reset();
  if (window_->GetDispatcher())
    window_->GetDispatcher()->RemoveRootWindowObserver(this);
  UnlockMouse();
  if (popup_parent_host_view_) {
    DCHECK(popup_parent_host_view_->popup_child_host_view_ == NULL ||
           popup_parent_host_view_->popup_child_host_view_ == this);
    popup_parent_host_view_->popup_child_host_view_ = NULL;
  }
  if (popup_child_host_view_) {
    DCHECK(popup_child_host_view_->popup_parent_host_view_ == NULL ||
           popup_child_host_view_->popup_parent_host_view_ == this);
    popup_child_host_view_->popup_parent_host_view_ = NULL;
  }
  aura::client::SetTooltipText(window_, NULL);
  gfx::Screen::GetScreenFor(window_)->RemoveObserver(this);

  DetachFromInputMethod();

  if (resource_collection_.get())
    resource_collection_->SetClient(NULL);

  for (std::set<OwnedMailbox*>::iterator it =
           active_frame_subscriber_textures_.begin();
       it != active_frame_subscriber_textures_.end();
       ++it) {
    (*it)->Destroy();
  }
  active_frame_subscriber_textures_.clear();

#if defined(OS_WIN)
  if (::IsWindow(plugin_parent_window_))
    ::DestroyWindow(plugin_parent_window_);
#endif
}
