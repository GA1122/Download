void GpuProcessHost::DidLoseContext(bool offscreen,
                                    gpu::error::ContextLostReason reason,
                                    const GURL& active_url) {
  TRACE_EVENT2("gpu", "GpuProcessHost::DidLoseContext", "reason", reason, "url",
               active_url.possibly_invalid_spec());

  if (!offscreen || active_url.is_empty()) {
    if (!dont_disable_webgl_when_compositor_context_lost_) {
      BlockLiveOffscreenContexts();
    }
    return;
  }

  GpuDataManagerImpl::DomainGuilt guilt =
      GpuDataManagerImpl::DOMAIN_GUILT_UNKNOWN;
  switch (reason) {
    case gpu::error::kGuilty:
      guilt = GpuDataManagerImpl::DOMAIN_GUILT_KNOWN;
      break;
    case gpu::error::kUnknown:
    case gpu::error::kOutOfMemory:
    case gpu::error::kMakeCurrentFailed:
    case gpu::error::kGpuChannelLost:
    case gpu::error::kInvalidGpuMessage:
      break;
    case gpu::error::kInnocent:
      return;
  }

  GpuDataManagerImpl::GetInstance()->BlockDomainFrom3DAPIs(active_url, guilt);
}
