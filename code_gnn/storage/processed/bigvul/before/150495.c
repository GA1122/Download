std::string SafeBrowsingPrivateEventRouter::GetProfileUserName() {
  return identity_manager_ && identity_manager_->HasPrimaryAccount()
             ? identity_manager_->GetPrimaryAccountInfo().email
             : std::string();
}
