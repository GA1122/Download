void AllViewsStoppedLoadingObserver::OnNoMorePendingLoads(
    content::WebContents* web_contents) {
  if (!automation_) {
    delete this;
    return;
  }

  TabSet::iterator iter = pending_tabs_.find(web_contents);
  if (iter == pending_tabs_.end()) {
    LOG(ERROR) << "Received OnNoMorePendingLoads for tab without "
               << "OnFirstPendingLoad.";
    return;
  }
  pending_tabs_.erase(iter);
  CheckIfNoMorePendingLoads();
}
