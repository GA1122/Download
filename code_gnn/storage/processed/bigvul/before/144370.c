size_t ExtensionInstallPrompt::Prompt::GetPermissionsDetailsCount(
    PermissionsType permissions_type) const {
  switch (permissions_type) {
    case REGULAR_PERMISSIONS:
      return prompt_permissions_.details.size();
    case WITHHELD_PERMISSIONS:
      return withheld_prompt_permissions_.details.size();
    case ALL_PERMISSIONS:
      return prompt_permissions_.details.size() +
             withheld_prompt_permissions_.details.size();
    default:
      NOTREACHED();
      return 0u;
  }
}
