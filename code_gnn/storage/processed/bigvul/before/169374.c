bool WaitForLoadStop(WebContents* web_contents) {
  WebContentsDestroyedObserver observer(web_contents);
  WaitForLoadStopWithoutSuccessCheck(web_contents);
  if (observer.IsDestroyed()) {
    LOG(ERROR) << "WebContents was destroyed during waiting for load stop.";
    return false;
  }
  return IsLastCommittedEntryOfPageType(web_contents, PAGE_TYPE_NORMAL);
}
