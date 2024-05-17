bool ShouldShowExtension(const Extension* extension) {
  if (extension->is_theme())
    return false;

  if (extension->location() == Extension::COMPONENT &&
      !CommandLine::ForCurrentProcess()->HasSwitch(
        switches::kShowComponentExtensionOptions))
    return false;

  if (extension->location() == Extension::LOAD)
    return true;

  if (extension->is_hosted_app())
    return false;

  return true;
}
