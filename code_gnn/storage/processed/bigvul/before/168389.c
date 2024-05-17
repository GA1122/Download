BrowserView::~BrowserView() {
  DCHECK_EQ(0, browser_->tab_strip_model()->count());

  loading_animation_timer_.Stop();

  immersive_mode_controller_.reset();

  browser_->tab_strip_model()->RemoveObserver(this);

  extensions::ExtensionCommandsGlobalRegistry* global_registry =
      extensions::ExtensionCommandsGlobalRegistry::Get(browser_->profile());
  if (global_registry->registry_for_active_window() ==
          extension_keybinding_registry_.get())
    global_registry->set_registry_for_active_window(nullptr);

  BrowserViewLayout* browser_view_layout = GetBrowserViewLayout();
  if (browser_view_layout)
    browser_view_layout->set_download_shelf(nullptr);
  download_shelf_.reset();

  if (tabstrip_) {
    tabstrip_->parent()->RemoveChildView(tabstrip_);
    if (browser_view_layout)
      browser_view_layout->set_tab_strip(nullptr);
    delete tabstrip_;
    tabstrip_ = nullptr;
  }
  RemoveAllChildViews(true);
  toolbar_ = nullptr;
}
