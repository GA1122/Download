void WebContentsLoadedOrDestroyedWatcher::DidStopLoading() {
  message_loop_runner_->Quit();
}
