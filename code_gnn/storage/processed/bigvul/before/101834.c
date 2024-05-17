TabContentsWrapper* Browser::TabContentsFactory(
    Profile* profile,
    SiteInstance* site_instance,
    int routing_id,
    const TabContents* base_tab_contents,
    SessionStorageNamespace* session_storage_namespace) {
  TabContents* new_contents = new TabContents(profile, site_instance,
                                              routing_id, base_tab_contents,
                                              session_storage_namespace);
  TabContentsWrapper* wrapper = new TabContentsWrapper(new_contents);
  return wrapper;
}
