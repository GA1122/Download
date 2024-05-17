void Shell::PlatformInitialize(const gfx::Size& default_window_size) {
  CHECK(!platform_);
  aura::Env::CreateInstance();
  platform_ = new ShellAuraPlatformData();
}
