std::vector<string16> Extension::GetPermissionMessageStrings() const {
  if (IsTrustedId(id_))
    return std::vector<string16>();
  else
    return permission_set_->GetWarningMessages();
}
