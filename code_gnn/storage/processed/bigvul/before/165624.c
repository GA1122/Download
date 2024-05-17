void Location::reload(LocalDOMWindow* current_window) {
  if (!IsAttached())
    return;
  if (GetDocument()->Url().ProtocolIsJavaScript())
    return;
  ToLocalDOMWindow(dom_window_)
      ->GetFrame()
      ->Reload(WebFrameLoadType::kReload,
               ClientRedirectPolicy::kClientRedirect);
}
