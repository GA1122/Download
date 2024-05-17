bool RenderFrameHostImpl::IsSandboxed(blink::WebSandboxFlags flags) const {
  return static_cast<int>(active_sandbox_flags_) & static_cast<int>(flags);
}
