BrowserView::BrowserView(Browser* browser)
    : views::ClientView(NULL, NULL),
      last_focused_view_storage_id_(
          views::ViewStorage::GetInstance()->CreateStorageID()),
      frame_(NULL),
      browser_(browser),
      active_bookmark_bar_(NULL),
      tabstrip_(NULL),
      toolbar_(NULL),
      window_switcher_button_(NULL),
      infobar_container_(NULL),
      contents_container_(NULL),
      devtools_container_(NULL),
      contents_(NULL),
      contents_split_(NULL),
      devtools_dock_side_(DEVTOOLS_DOCK_SIDE_BOTTOM),
      devtools_window_(NULL),
      initialized_(false),
      ignore_layout_(true),
#if defined(OS_WIN) && !defined(USE_AURA)
      hung_window_detector_(&hung_plugin_action_),
      ticker_(0),
#endif
      force_location_bar_focus_(false),
      ALLOW_THIS_IN_INITIALIZER_LIST(color_change_listener_(this)),
      ALLOW_THIS_IN_INITIALIZER_LIST(activate_modal_dialog_factory_(this)) {
  browser_->tab_strip_model()->AddObserver(this);
}
