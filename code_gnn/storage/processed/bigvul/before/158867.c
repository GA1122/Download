void Browser::TabDetachedAt(WebContents* contents, int index, bool was_active) {
  if (!tab_strip_model_->closing_all()) {
    SessionService* session_service =
        SessionServiceFactory::GetForProfileIfExisting(profile_);
    if (session_service) {
      session_service->SetSelectedTabInWindow(session_id(),
                                              tab_strip_model_->active_index());
    }
  }

  TabDetachedAtImpl(contents, was_active, DETACH_TYPE_DETACH);
}
