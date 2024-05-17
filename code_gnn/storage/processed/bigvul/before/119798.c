NavigationControllerImpl::NavigationControllerImpl(
    WebContentsImpl* web_contents,
    BrowserContext* browser_context)
    : browser_context_(browser_context),
      pending_entry_(NULL),
      last_committed_entry_index_(-1),
      pending_entry_index_(-1),
      transient_entry_index_(-1),
      web_contents_(web_contents),
      max_restored_page_id_(-1),
      ssl_manager_(this),
      needs_reload_(false),
      is_initial_navigation_(true),
      pending_reload_(NO_RELOAD),
      get_timestamp_callback_(base::Bind(&base::Time::Now)),
      screenshot_manager_(new WebContentsScreenshotManager(this)) {
  DCHECK(browser_context_);
}
