ContentRendererClient* ShellMainDelegate::CreateContentRendererClient() {
  renderer_client_.reset(switches::IsRunWebTestsSwitchPresent()
                             ? new LayoutTestContentRendererClient
                             : new ShellContentRendererClient);

  return renderer_client_.get();
 }
