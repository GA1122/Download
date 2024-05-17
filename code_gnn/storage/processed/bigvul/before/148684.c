void SkiaOutputSurfaceImplTest::SetUpSkiaOutputSurfaceImpl() {
  const char enable_features[] = "VizDisplayCompositor,UseSkiaRenderer";
  const char disable_features[] = "";
  scoped_feature_list_ = std::make_unique<base::test::ScopedFeatureList>();
  scoped_feature_list_->InitFromCommandLine(enable_features, disable_features);
  gpu_service_holder_ = TestGpuServiceHolder::GetInstance();

  gpu::SurfaceHandle surface_handle_ = gpu::kNullSurfaceHandle;
  if (on_screen_) {
#if BUILDFLAG(ENABLE_VULKAN) && defined(USE_X11)
    surface_handle_ = gpu::CreateNativeWindow(kSurfaceRect);
#else
    NOTREACHED();
#endif
  }
  output_surface_ = SkiaOutputSurfaceImpl::Create(
      std::make_unique<SkiaOutputSurfaceDependencyImpl>(gpu_service(),
                                                        surface_handle_),
      RendererSettings());
  output_surface_->BindToClient(output_surface_client_.get());
}
