void ChromeContentBrowserClient::ExposeInterfacesToMediaService(
    service_manager::BinderRegistry* registry,
    content::RenderFrameHost* render_frame_host) {
#if BUILDFLAG(ENABLE_LIBRARY_CDMS)
  registry->AddInterface(
      base::Bind(&OutputProtectionImpl::Create, render_frame_host));
  registry->AddInterface(
      base::Bind(&PlatformVerificationImpl::Create, render_frame_host));
#endif   

#if BUILDFLAG(ENABLE_MOJO_CDM) && defined(OS_ANDROID)
  registry->AddInterface(base::Bind(&CreateMediaDrmStorage, render_frame_host));
#endif
}
