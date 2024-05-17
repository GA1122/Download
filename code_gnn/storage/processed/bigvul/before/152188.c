void RenderFrameImpl::AllowBindings(int32_t enabled_bindings_flags) {
  if (IsMainFrame() && (enabled_bindings_flags & BINDINGS_POLICY_WEB_UI) &&
      !(enabled_bindings_ & BINDINGS_POLICY_WEB_UI)) {
    new WebUIExtensionData(render_view_);
  }

  enabled_bindings_ |= enabled_bindings_flags;

  RenderProcess::current()->AddBindings(enabled_bindings_flags);
}
