bool SupervisedUserService::MustRemainInstalled(const Extension* extension,
                                                base::string16* error) const {
  DCHECK(ProfileIsSupervised());
  ExtensionState result = GetExtensionState(*extension);
  bool may_not_uninstall = result == ExtensionState::FORCED;
  if (may_not_uninstall && error)
    *error = GetExtensionsLockedMessage();
  return may_not_uninstall;
}
