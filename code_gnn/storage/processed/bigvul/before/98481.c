void AeroPeekWindow::OnActivate(UINT action,
                                BOOL minimized,
                                HWND window) {
  if (action == WA_INACTIVE)
    return;

  if (delegate_)
    delegate_->ActivateTab(tab_id_);
}
