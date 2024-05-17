void VaapiWrapper::PreSandboxInitialization() {
#if defined(USE_OZONE)
  const char* kDriRenderNode0Path = "/dev/dri/renderD128";
  base::File drm_file = base::File(
      base::FilePath::FromUTF8Unsafe(kDriRenderNode0Path),
      base::File::FLAG_OPEN | base::File::FLAG_READ | base::File::FLAG_WRITE);
  if (drm_file.IsValid())
    va_display_state_.Get().SetDrmFd(drm_file.GetPlatformFile());
#endif
}
