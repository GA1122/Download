 void UnloadController::ProcessPendingTabs() {
  if (!is_attempting_to_close_browser_) {
    return;
  }

  if (HasCompletedUnloadProcessing()) {
    browser_->OnWindowClosing();
    return;
  }

  if (!tabs_needing_before_unload_fired_.empty()) {
    content::WebContents* web_contents =
        *(tabs_needing_before_unload_fired_.begin());
    if (web_contents->GetRenderViewHost()) {
      web_contents->GetRenderViewHost()->FirePageBeforeUnload(false);
    } else {
      ClearUnloadState(web_contents, true);
    }
  } else if (!tabs_needing_unload_fired_.empty()) {
    content::WebContents* web_contents = *(tabs_needing_unload_fired_.begin());
    if (web_contents->GetRenderViewHost()) {
      web_contents->GetRenderViewHost()->ClosePage();
    } else {
      ClearUnloadState(web_contents, true);
    }
  } else {
    NOTREACHED();
  }
}
