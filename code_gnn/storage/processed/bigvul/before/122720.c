bool Extension::ShouldDisplayInAppLauncher() const {
  return is_app() && display_in_launcher_;
}
