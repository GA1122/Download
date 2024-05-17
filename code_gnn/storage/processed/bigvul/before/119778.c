NavigationControllerImpl::GetSessionStorageNamespace(SiteInstance* instance) {
  std::string partition_id;
  if (instance) {
    partition_id =
        GetContentClient()->browser()->GetStoragePartitionIdForSite(
            browser_context_, instance->GetSiteURL());
  }

  SessionStorageNamespaceMap::const_iterator it =
      session_storage_namespace_map_.find(partition_id);
  if (it != session_storage_namespace_map_.end())
    return it->second.get();

  StoragePartition* partition =
              BrowserContext::GetStoragePartition(browser_context_, instance);
  SessionStorageNamespaceImpl* session_storage_namespace =
      new SessionStorageNamespaceImpl(
          static_cast<DOMStorageContextWrapper*>(
              partition->GetDOMStorageContext()));
  session_storage_namespace_map_[partition_id] = session_storage_namespace;

  return session_storage_namespace;
}