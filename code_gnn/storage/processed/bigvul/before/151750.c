void Browser::TabDetachedAt(WebContents* contents, int index) {
  int old_active_index = tab_strip_model_->active_index();
  if (index < old_active_index && !tab_strip_model_->closing_all()) {
    SessionService* session_service =
        SessionServiceFactory::GetForProfileIfExisting(profile_);
    if (session_service)
      session_service->SetSelectedTabInWindow(session_id(),
                                              old_active_index - 1);
  }

  TabDetachedAtImpl(contents, index, DETACH_TYPE_DETACH);
}
