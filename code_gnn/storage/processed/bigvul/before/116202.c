void SSLManager::DidChangeSSLInternalState() {
  UpdateEntry(
      NavigationEntryImpl::FromNavigationEntry(controller_->GetActiveEntry()));
}
