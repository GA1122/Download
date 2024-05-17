NavigationState::~NavigationState() {
  RunCommitNavigationCallback(blink::mojom::CommitResult::Aborted);
  navigation_client_.reset();
}
