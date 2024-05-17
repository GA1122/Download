bool SupervisedUserService::MustRemainDisabled(
    const Extension* extension,
    extensions::disable_reason::DisableReason* reason,
    base::string16* error) const {
  DCHECK(ProfileIsSupervised());
  ExtensionState state = GetExtensionState(*extension);
  bool must_remain_disabled = state == ExtensionState::REQUIRE_APPROVAL;

  if (must_remain_disabled) {
    if (error)
      *error = GetExtensionsLockedMessage();
    ExtensionPrefs* extension_prefs = ExtensionPrefs::Get(profile_);
    if (extension_prefs->HasDisableReason(
            extension->id(),
            extensions::disable_reason::DISABLE_PERMISSIONS_INCREASE)) {
      if (reason)
        *reason = extensions::disable_reason::DISABLE_PERMISSIONS_INCREASE;
      return true;
    }
    if (reason)
      *reason = extensions::disable_reason::DISABLE_CUSTODIAN_APPROVAL_REQUIRED;
    if (base::FeatureList::IsEnabled(
            supervised_users::kSupervisedUserInitiatedExtensionInstall)) {
      if (!extension_prefs->HasDisableReason(
              extension->id(), extensions::disable_reason::
                                   DISABLE_CUSTODIAN_APPROVAL_REQUIRED)) {
        SupervisedUserService* supervised_user_service =
            SupervisedUserServiceFactory::GetForProfile(profile_);
        supervised_user_service->AddExtensionInstallRequest(
            extension->id(), extension->version());
      }
    }
  }
  return must_remain_disabled;
}
