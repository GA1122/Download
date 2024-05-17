void LocalDOMWindow::RemovePostMessageTimer(PostMessageTimer* timer) {
  post_message_timers_.erase(timer);
}
