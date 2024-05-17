void BrowsingDataExtensionFunction::StartRemoving() {
  AddRef();

  BrowsingDataRemover* remover = new BrowsingDataRemover(
      GetCurrentBrowser()->profile(), remove_since_, base::Time::Now());
  remover->AddObserver(this);
  remover->Remove(removal_mask_);
}
