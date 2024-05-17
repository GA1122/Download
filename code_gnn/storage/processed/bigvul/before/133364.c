aura::Window* GetWindow(AshWindowTreeHost* ash_host) {
  CHECK(ash_host->AsWindowTreeHost());
  return ash_host->AsWindowTreeHost()->window();
}
