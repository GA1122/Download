void Browser::DuplicateContentsAt(int index) {
  TabContentsWrapper* contents = GetTabContentsWrapperAt(index);
  CHECK(contents);
  TabContentsWrapper* contents_dupe = contents->Clone();

  bool pinned = false;
  if (CanSupportWindowFeature(FEATURE_TABSTRIP)) {
    int index = tab_handler_->GetTabStripModel()->
        GetIndexOfTabContents(contents);
    pinned = tab_handler_->GetTabStripModel()->IsTabPinned(index);
    int add_types = TabStripModel::ADD_ACTIVE |
        TabStripModel::ADD_INHERIT_GROUP |
        (pinned ? TabStripModel::ADD_PINNED : 0);
    tab_handler_->GetTabStripModel()->InsertTabContentsAt(index + 1,
                                                          contents_dupe,
                                                          add_types);
  } else {
    Browser* browser = NULL;
    if (is_app()) {
      CHECK(!is_type_popup());
      CHECK(!is_type_panel());
      browser = Browser::CreateForApp(TYPE_POPUP, app_name_, gfx::Rect(),
                                      profile_);
    } else if (is_type_popup()) {
      browser = Browser::CreateForType(TYPE_POPUP, profile_);
    }

    BrowserWindow* new_window = browser->window();
    new_window->SetBounds(gfx::Rect(new_window->GetRestoredBounds().origin(),
                          window()->GetRestoredBounds().size()));

    browser->window()->Show();

    browser->AddTab(contents_dupe, PageTransition::LINK);
  }

  SessionService* session_service =
      SessionServiceFactory::GetForProfileIfExisting(profile_);
  if (session_service)
    session_service->TabRestored(contents_dupe, pinned);
}
