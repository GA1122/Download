void RenderViewImpl::OnAllowBindings(int enabled_bindings_flags) {
  if ((enabled_bindings_flags & BINDINGS_POLICY_WEB_UI) &&
      !(enabled_bindings_ & BINDINGS_POLICY_WEB_UI)) {
    RenderThread::Get()->RegisterExtension(WebUIExtension::Get());
    new WebUIExtensionData(this);
  }

  enabled_bindings_ |= enabled_bindings_flags;

  RenderProcess::current()->AddBindings(enabled_bindings_flags);
}
