void RenderViewImpl::OnAllowBindings(int enabled_bindings_flags) {
  enabled_bindings_ |= enabled_bindings_flags;

  RenderProcess::current()->AddBindings(enabled_bindings_flags);
}