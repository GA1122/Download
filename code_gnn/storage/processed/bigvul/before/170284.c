void WebContentsLoadedOrDestroyedWatcher::WebContentsDestroyed() {
  message_loop_runner_->Quit();
}
