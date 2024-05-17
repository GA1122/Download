void BrowserView::LoadingAnimationCallback() {
  if (browser_->is_type_tabbed()) {
    tabstrip_->UpdateLoadingAnimations();
  } else if (ShouldShowWindowIcon()) {
    WebContents* web_contents =
        browser_->tab_strip_model()->GetActiveWebContents();
    frame_->UpdateThrobber(web_contents && web_contents->IsLoading());
  }
}
