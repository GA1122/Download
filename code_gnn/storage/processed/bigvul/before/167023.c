NavigationControllerImpl::NavigationControllerImpl(
    NavigationControllerDelegate* delegate,
    BrowserContext* browser_context)
    : browser_context_(browser_context),
      pending_entry_(nullptr),
      failed_pending_entry_id_(0),
      last_committed_entry_index_(-1),
      pending_entry_index_(-1),
      transient_entry_index_(-1),
      delegate_(delegate),
      ssl_manager_(this),
      needs_reload_(false),
      is_initial_navigation_(true),
      in_navigate_to_pending_entry_(false),
      pending_reload_(ReloadType::NONE),
      get_timestamp_callback_(base::Bind(&base::Time::Now)),
      screenshot_manager_(new NavigationEntryScreenshotManager(this)),
      last_committed_reload_type_(ReloadType::NONE) {
  DCHECK(browser_context_);
}
