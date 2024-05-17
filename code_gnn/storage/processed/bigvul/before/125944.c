bool NavigationControllerRestoredObserver::FinishedRestoring() {
  return (!controller_->NeedsReload() && !controller_->GetPendingEntry() &&
          !controller_->GetWebContents()->IsLoading());
}
