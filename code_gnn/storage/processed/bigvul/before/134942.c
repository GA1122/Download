bool ShouldHideActiveAppsFromShelf() {
  return base::CommandLine::ForCurrentProcess()->HasSwitch(
      kHideActiveAppsFromShelf);
}
