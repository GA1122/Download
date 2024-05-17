void HWNDMessageHandler::PostProcessActivateMessage(int activation_state,
                                                    bool minimized) {
  DCHECK(delegate_->CanSaveFocus());

  bool active = activation_state != WA_INACTIVE && !minimized;
  if (delegate_->CanActivate())
    delegate_->HandleActivationChanged(active);

  if (!active) {
    restore_focus_when_enabled_ = false;
    delegate_->SaveFocusOnDeactivate();
  } else {
    if (!IsWindowEnabled(hwnd())) {
      DCHECK(!restore_focus_when_enabled_);
      restore_focus_when_enabled_ = true;
      return;
    }
    delegate_->RestoreFocusOnActivate();
  }
}
