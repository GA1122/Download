void WebContentsViewAura::CreateView(
    const gfx::Size& initial_size, gfx::NativeView context) {

  aura::Env::CreateInstance();
  window_.reset(new aura::Window(this));
  window_->set_owned_by_parent(false);
  window_->SetType(ui::wm::WINDOW_TYPE_CONTROL);
  window_->SetTransparent(false);
  window_->Init(aura::WINDOW_LAYER_NOT_DRAWN);
  aura::Window* root_window = context ? context->GetRootWindow() : NULL;
  if (root_window) {
    if (root_window) {
      aura::client::ParentWindowWithContext(
          window_.get(), root_window, root_window->GetBoundsInScreen());
    }
  }
  window_->layer()->SetMasksToBounds(true);
  window_->SetName("WebContentsViewAura");

  if (!web_contents_->GetRenderProcessHost()->IsGuest())
    window_observer_.reset(new WindowObserver(this));

  if (delegate_)
    drag_dest_delegate_ = delegate_->GetDragDestDelegate();
}
