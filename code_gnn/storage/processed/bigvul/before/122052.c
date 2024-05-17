void Shell::PlatformExit() {
  CHECK(platform_);
  delete platform_;
  platform_ = NULL;
  aura::Env::DeleteInstance();
}
