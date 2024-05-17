void BrowserWindowGtk::LoadingAnimationCallback() {
  if (browser_->is_type_tabbed()) {
    tabstrip_->UpdateLoadingAnimations();
  } else if (ShouldShowWindowIcon()) {
    WebContents* web_contents = chrome::GetActiveWebContents(browser_.get());
    titlebar_->UpdateThrobber(web_contents);
  }
}
