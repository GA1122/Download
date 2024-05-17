int BrowserMainParts::PreCreateThreads() {
  gfx::InitializeOxideNativeDisplay(
      BrowserPlatformIntegration::GetInstance()->GetNativeDisplay());

  {
    ScopedBindGLESAPI gles_binder;

    gl::init::InitializeGLOneOff();
  }

  device_client_.reset(new DeviceClient());

  primary_screen_.reset(new display::Screen());
  ::display::Screen::SetScreenInstance(primary_screen_.get());

  io_thread_.reset(new IOThread());

  return 0;
}
