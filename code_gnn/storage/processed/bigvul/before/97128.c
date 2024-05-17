NavigationController::NavigationController(TabContents* contents,
                                           Profile* profile)
    : profile_(profile),
      pending_entry_(NULL),
      last_committed_entry_index_(-1),
      pending_entry_index_(-1),
      transient_entry_index_(-1),
      tab_contents_(contents),
      max_restored_page_id_(-1),
      ALLOW_THIS_IN_INITIALIZER_LIST(ssl_manager_(this)),
      needs_reload_(false),
      user_gesture_observed_(false) {
  DCHECK(profile_);
}
