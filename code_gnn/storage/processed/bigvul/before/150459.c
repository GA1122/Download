NavigationControllerImpl::GetSessionStorageNamespace(SiteInstance* instance) {
  std::string partition_id;
  if (instance) {
    partition_id =
        GetContentClient()->browser()->GetStoragePartitionIdForSite(
            browser_context_, instance->GetSiteURL());
  }

  StoragePartition* partition =
      BrowserContext::GetStoragePartition(browser_context_, instance);
  DOMStorageContextWrapper* context_wrapper =
      static_cast<DOMStorageContextWrapper*>(partition->GetDOMStorageContext());

  SessionStorageNamespaceMap::const_iterator it =
      session_storage_namespace_map_.find(partition_id);
  if (it != session_storage_namespace_map_.end()) {
    DCHECK(static_cast<SessionStorageNamespaceImpl*>(it->second.get())->
        IsFromContext(context_wrapper));
    return it->second.get();
  }

  SessionStorageNamespaceImpl* session_storage_namespace =
      new SessionStorageNamespaceImpl(context_wrapper);
  session_storage_namespace_map_[partition_id] = session_storage_namespace;

  return session_storage_namespace;
}
