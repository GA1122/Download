bool ShouldReleaseFrontSurface() {
  static bool release_front_surface_allowed =
      CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kEnableUIReleaseFrontSurface);
  return release_front_surface_allowed;
}
