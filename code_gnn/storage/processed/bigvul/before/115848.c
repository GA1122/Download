NavigationController::NavigationController(
    TabContents* contents,
    content::BrowserContext* browser_context,
    SessionStorageNamespace* session_storage_namespace)
    : browser_context_(browser_context),
      pending_entry_(NULL),
      last_committed_entry_index_(-1),
      pending_entry_index_(-1),
      transient_entry_index_(-1),
      tab_contents_(contents),
      max_restored_page_id_(-1),
      ALLOW_THIS_IN_INITIALIZER_LIST(ssl_manager_(this)),
      needs_reload_(false),
      session_storage_namespace_(session_storage_namespace),
      pending_reload_(NO_RELOAD) {
  DCHECK(browser_context_);
  if (!session_storage_namespace_) {
    session_storage_namespace_ = new SessionStorageNamespace(
        browser_context_->GetWebKitContext());
  }
}
