void ImageTransportFactory::Initialize() {
  CommandLine* command_line = CommandLine::ForCurrentProcess();
  if (command_line->HasSwitch(switches::kTestCompositor)) {
    ui::SetupTestCompositor();
  }
  if (ui::IsTestCompositorEnabled()) {
    g_factory = new DefaultTransportFactory();
    WebKitPlatformSupportImpl::SetOffscreenContextFactoryForTest(
        CreateTestContext);
  } else {
    g_factory = new GpuProcessTransportFactory();
  }
  ui::ContextFactory::SetInstance(g_factory->AsContextFactory());
}
