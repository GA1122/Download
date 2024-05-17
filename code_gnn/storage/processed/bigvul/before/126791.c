gfx::ImageSkia BrowserView::GetWindowAppIcon() {
  if (browser_->is_app()) {
    WebContents* contents = chrome::GetActiveWebContents(browser_.get());
    extensions::TabHelper* extensions_tab_helper =
        contents ? extensions::TabHelper::FromWebContents(contents) : NULL;
    if (extensions_tab_helper && extensions_tab_helper->GetExtensionAppIcon())
      return gfx::ImageSkia(*extensions_tab_helper->GetExtensionAppIcon());
  }

  return GetWindowIcon();
}
