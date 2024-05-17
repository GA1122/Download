LayoutTestContentRendererClient::LayoutTestContentRendererClient() {
  EnableWebTestProxyCreation(
      base::Bind(&LayoutTestContentRendererClient::WebTestProxyCreated,
                 base::Unretained(this)));

#if defined(OS_WIN)
  if (gfx::win::ShouldUseDirectWrite())
    RegisterSideloadedTypefaces(GetPreSandboxWarmupFontMgr());
#endif
}
