bool Extension::ShouldDisplayInExtensionSettings() const {
  if (is_theme())
    return false;

  if (location() == Manifest::COMPONENT &&
      !CommandLine::ForCurrentProcess()->HasSwitch(
        switches::kShowComponentExtensionOptions)) {
    return false;
  }

  if (location() == Manifest::LOAD)
    return true;

  if (is_hosted_app())
    return false;

  return true;
}
