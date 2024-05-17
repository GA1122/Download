void NavigationState::RunCommitNavigationCallback(
    blink::mojom::CommitResult result) {
  if (commit_callback_)
    std::move(commit_callback_).Run(result);
}
