void SSLManager::DidRunInsecureContent(const std::string& security_origin) {
  policy()->DidRunInsecureContent(
      NavigationEntryImpl::FromNavigationEntry(controller_->GetActiveEntry()),
      security_origin);
}
