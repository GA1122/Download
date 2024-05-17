void TargetHandler::DidCommitNavigation() {
  auto_attacher_.UpdateServiceWorkers();
}
