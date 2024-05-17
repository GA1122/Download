std::string ExtensionInstallPrompt::PromptTypeToString(PromptType type) {
  switch (type) {
    case ExtensionInstallPrompt::INSTALL_PROMPT:
      return "INSTALL_PROMPT";
    case ExtensionInstallPrompt::INLINE_INSTALL_PROMPT:
      return "INLINE_INSTALL_PROMPT";
    case ExtensionInstallPrompt::BUNDLE_INSTALL_PROMPT:
      return "BUNDLE_INSTALL_PROMPT";
    case ExtensionInstallPrompt::RE_ENABLE_PROMPT:
      return "RE_ENABLE_PROMPT";
    case ExtensionInstallPrompt::PERMISSIONS_PROMPT:
      return "PERMISSIONS_PROMPT";
    case ExtensionInstallPrompt::EXTERNAL_INSTALL_PROMPT:
      return "EXTERNAL_INSTALL_PROMPT";
    case ExtensionInstallPrompt::POST_INSTALL_PERMISSIONS_PROMPT:
      return "POST_INSTALL_PERMISSIONS_PROMPT";
    case ExtensionInstallPrompt::REMOTE_INSTALL_PROMPT:
      return "REMOTE_INSTALL_PROMPT";
    case ExtensionInstallPrompt::REPAIR_PROMPT:
      return "REPAIR_PROMPT";
    case ExtensionInstallPrompt::DELEGATED_PERMISSIONS_PROMPT:
      return "DELEGATED_PERMISSIONS_PROMPT";
    case ExtensionInstallPrompt::DELEGATED_BUNDLE_PERMISSIONS_PROMPT:
      return "DELEGATED_BUNDLE_PERMISSIONS_PROMPT";
    case ExtensionInstallPrompt::LAUNCH_PROMPT_DEPRECATED:
      NOTREACHED();
    case ExtensionInstallPrompt::UNSET_PROMPT_TYPE:
    case ExtensionInstallPrompt::NUM_PROMPT_TYPES:
      break;
  }
  return "OTHER";
}
