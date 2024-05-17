ChromeNetworkDelegate::ChromeNetworkDelegate(
    extensions::EventRouterForwarder* event_router,
    BooleanPrefMember* enable_referrers)
    : event_router_(event_router),
      profile_(NULL),
      enable_referrers_(enable_referrers),
      enable_do_not_track_(NULL),
      force_google_safe_search_(NULL),
      url_blacklist_manager_(NULL),
      load_time_stats_(NULL),
      received_content_length_(0),
      original_content_length_(0) {
  DCHECK(event_router);
  DCHECK(enable_referrers);
}
