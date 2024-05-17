    BrowserPolicyConnector::LockDevice(const std::string& user) {
#if defined(OS_CHROMEOS)
  if (install_attributes_.get())
    return install_attributes_->LockDevice(user);
#endif

  return EnterpriseInstallAttributes::LOCK_BACKEND_ERROR;
}
