void ClientControlledShellSurface::SetImeBlocked(bool ime_blocked) {
  TRACE_EVENT1("exo", "ClientControlledShellSurface::SetImeBlocked",
               "ime_blocked", ime_blocked);

  if (!widget_)
    CreateShellSurfaceWidget(ui::SHOW_STATE_NORMAL);

  WMHelper::GetInstance()->SetImeBlocked(widget_->GetNativeWindow(),
                                         ime_blocked);
}
