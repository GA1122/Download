WebSandboxFlags WebLocalFrameImpl::EffectiveSandboxFlags() const {
  if (!GetFrame())
    return WebSandboxFlags::kNone;
  return static_cast<WebSandboxFlags>(
      GetFrame()->Loader().EffectiveSandboxFlags());
}
