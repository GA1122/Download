bool Extension::IsDisallowedExperimentalPermission(
    ExtensionAPIPermission::ID permission) const {
  return permission == ExtensionAPIPermission::kExperimental &&
      !CommandLine::ForCurrentProcess()->HasSwitch(
        switches::kEnableExperimentalExtensionApis);
}
