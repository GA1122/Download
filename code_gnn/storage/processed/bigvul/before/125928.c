void AllViewsStoppedLoadingObserver::CheckIfNoMorePendingLoads() {
  if (!automation_) {
    delete this;
    return;
  }

  if (pending_tabs_.empty() &&
      DidExtensionViewsStopLoading(extension_process_manager_)) {
    AutomationJSONReply(automation_,
                        reply_message_.release()).SendSuccess(NULL);
    delete this;
  }
}
