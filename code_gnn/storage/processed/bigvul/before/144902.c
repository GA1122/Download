void RenderWidgetHostViewAura::FocusedNodeChanged(
    bool editable,
    const gfx::Rect& node_bounds_in_screen) {
#if defined(OS_WIN)
  if (!editable && virtual_keyboard_requested_) {
    virtual_keyboard_requested_ = false;

    RenderViewHost* rvh = RenderViewHost::From(host_);
    if (rvh && rvh->GetDelegate())
      rvh->GetDelegate()->SetIsVirtualKeyboardRequested(false);

    DCHECK(ui::OnScreenKeyboardDisplayManager::GetInstance());
    ui::OnScreenKeyboardDisplayManager::GetInstance()->DismissVirtualKeyboard();
  }
#endif
}
