void RenderMessageFilter::OnDidLose3DContext(
    const GURL& top_origin_url,
    ThreeDAPIType  ,
    int arb_robustness_status_code) {
#if defined(OS_MACOSX)
#ifndef GL_GUILTY_CONTEXT_RESET_ARB
#define GL_GUILTY_CONTEXT_RESET_ARB 0x8253
#endif
#ifndef GL_INNOCENT_CONTEXT_RESET_ARB
#define GL_INNOCENT_CONTEXT_RESET_ARB 0x8254
#endif
#ifndef GL_UNKNOWN_CONTEXT_RESET_ARB
#define GL_UNKNOWN_CONTEXT_RESET_ARB 0x8255
#endif

#endif
  GpuDataManagerImpl::DomainGuilt guilt;
  switch (arb_robustness_status_code) {
    case GL_GUILTY_CONTEXT_RESET_ARB:
      guilt = GpuDataManagerImpl::DOMAIN_GUILT_KNOWN;
      break;
    case GL_UNKNOWN_CONTEXT_RESET_ARB:
      guilt = GpuDataManagerImpl::DOMAIN_GUILT_UNKNOWN;
      break;
    default:
      return;
  }

  GpuDataManagerImpl::GetInstance()->BlockDomainFrom3DAPIs(
      top_origin_url, guilt);
}
