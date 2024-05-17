bool CanUseSharedGLContext() {
#if defined(ENABLE_HYBRIS)
  if (!HybrisUtils::GetInstance()->IsUsingAndroidEGL()) {
    return true;
  }

  if (content::GpuDataManagerImpl::GetInstance()->IsDriverBugWorkaroundActive(
          gpu::USE_VIRTUALIZED_GL_CONTEXTS)) {
    return false;
  }
#endif

  return true;
}
