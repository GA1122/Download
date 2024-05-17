RenderWidgetHostViewAura::~RenderWidgetHostViewAura() {
  if (!is_guest_view_hack_)
    host_->ViewDestroyed();

  selection_controller_.reset();
  selection_controller_client_.reset();

  delegated_frame_host_.reset();
  window_observer_.reset();
  if (window_) {
    if (window_->GetHost())
      window_->GetHost()->RemoveObserver(this);
    UnlockMouse();
    aura::client::SetTooltipText(window_, NULL);
    display::Screen::GetScreen()->RemoveObserver(this);

    DetachFromInputMethod();
  }
  if (popup_parent_host_view_) {
    DCHECK(popup_parent_host_view_->popup_child_host_view_ == NULL ||
           popup_parent_host_view_->popup_child_host_view_ == this);
    popup_parent_host_view_->SetPopupChild(nullptr);
  }
  if (popup_child_host_view_) {
    DCHECK(popup_child_host_view_->popup_parent_host_view_ == NULL ||
           popup_child_host_view_->popup_parent_host_view_ == this);
    popup_child_host_view_->popup_parent_host_view_ = NULL;
  }
  event_filter_for_popup_exit_.reset();

#if defined(OS_WIN)
  DCHECK(!legacy_render_widget_host_HWND_);
  if (virtual_keyboard_requested_) {
    DCHECK(keyboard_observer_.get());
    ui::OnScreenKeyboardDisplayManager* osk_display_manager =
        ui::OnScreenKeyboardDisplayManager::GetInstance();
    DCHECK(osk_display_manager);
    osk_display_manager->RemoveObserver(keyboard_observer_.get());
  }

#endif

  if (text_input_manager_)
    text_input_manager_->RemoveObserver(this);
}
