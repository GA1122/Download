void BrowserView::Init(Browser* browser) {
  browser_.reset(browser);
  browser_->tab_strip_model()->AddObserver(this);
  immersive_mode_controller_.reset(chrome::CreateImmersiveModeController());
}
