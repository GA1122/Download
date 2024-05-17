void NavigationController::DocumentLoadedInFrame() {
  last_document_loaded_ = base::TimeTicks::Now();
}
