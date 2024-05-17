void BrowserMainParts::PreMainMessageLoopRun() {
  device::GeolocationProvider::SetGeolocationDelegate(
      new GeolocationDelegate());

  media::AudioManager::SetGlobalAppName(
      BrowserPlatformIntegration::GetInstance()->GetApplicationName());

  if (CanUseSharedGLContext()) {
    scoped_refptr<GLContextDependent> share_context =
        BrowserPlatformIntegration::GetInstance()->GetGLShareContext();
    if (share_context) {
      gl_share_context_ = GLContextDependent::CloneFrom(share_context.get());
      gpu::oxide_shim::SetGLShareGroup(gl_share_context_->share_group());
    }
  }

#if defined(ENABLE_HYBRIS_CAMERA)
  VideoCaptureDeviceHybris::Initialize();
#endif

  gpu::GPUInfo gpu_info;
  gpu::CollectInfoResult rv = gpu::CollectContextGraphicsInfo(&gpu_info);
  switch (rv) {
    case gpu::kCollectInfoFatalFailure:
      LOG(ERROR) << "gpu::CollectContextGraphicsInfo failed";
      break;
    case gpu::kCollectInfoNone:
      NOTREACHED();
      break;
    default:
      break;
  }
  content::GpuDataManagerImpl::GetInstance()->UpdateGpuInfo(gpu_info);

  CompositorUtils::GetInstance()->Initialize(gl_share_context_.get());
  net::NetModule::SetResourceProvider(NetResourceProvider);

  lifecycle_observer_.reset(new LifecycleObserver());
  render_process_initializer_.reset(new RenderProcessInitializer());
}
