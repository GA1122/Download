bool SupervisedUserService::UserMayModifySettings(const Extension* extension,
                                                  base::string16* error) const {
  DCHECK(ProfileIsSupervised());
  ExtensionState result = GetExtensionState(*extension);
  bool may_modify = result != ExtensionState::FORCED;
  if (!may_modify && error)
    *error = GetExtensionsLockedMessage();
  return may_modify;
}
