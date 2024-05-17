bool UnloadController::BeforeUnloadFired(content::WebContents* contents,
                                         bool proceed) {
  if (!is_attempting_to_close_browser_) {
    if (!proceed)
      contents->SetClosedByUserGesture(false);
    return proceed;
  }

  if (!proceed) {
    CancelWindowClose();
    contents->SetClosedByUserGesture(false);
    return false;
  }

  if (RemoveFromSet(&tabs_needing_before_unload_fired_, contents)) {
    tabs_needing_unload_fired_.insert(contents);
    ProcessPendingTabs();
    return false;
  }

  return true;
}
