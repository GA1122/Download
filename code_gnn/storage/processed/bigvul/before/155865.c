bool SupervisedUserService::UserMayLoad(const Extension* extension,
                                        base::string16* error) const {
  DCHECK(ProfileIsSupervised());
  ExtensionState result = GetExtensionState(*extension);
  bool may_load = result != ExtensionState::BLOCKED;
  if (!may_load && error)
    *error = GetExtensionsLockedMessage();
  return may_load;
}
