void WebContentsImpl::DidStopLoading() {
  std::unique_ptr<LoadNotificationDetails> details;

  NavigationEntry* entry = controller_.GetLastCommittedEntry();
  Navigator* navigator = frame_tree_.root()->navigator();

  if (entry) {
    base::TimeDelta elapsed =
        base::TimeTicks::Now() - navigator->GetCurrentLoadStart();

    details.reset(new LoadNotificationDetails(
        entry->GetVirtualURL(),
        elapsed,
        &controller_,
        controller_.GetCurrentEntryIndex()));
  }

  LoadingStateChanged(true, false, details.get());
}
