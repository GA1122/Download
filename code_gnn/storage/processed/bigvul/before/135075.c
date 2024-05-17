void AppCacheHost::NotifyMainResourceIsNamespaceEntry(
    const GURL& namespace_entry_url) {
  main_resource_was_namespace_entry_ = true;
  namespace_entry_url_ = namespace_entry_url;
}
