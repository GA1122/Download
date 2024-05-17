bool Extension::CanSpecifyExperimentalPermission() const {
  if (location() == Manifest::COMPONENT)
    return true;

  if (CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kEnableExperimentalExtensionApis)) {
    return true;
  }

  if (from_webstore())
    return true;

  return false;
}
