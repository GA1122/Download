void LocalDOMWindow::SchedulePostMessage(
    MessageEvent* event,
    scoped_refptr<const SecurityOrigin> target,
    Document* source) {
  std::unique_ptr<SourceLocation> location = SourceLocation::Capture(source);
  PostMessageTimer* timer =
      new PostMessageTimer(*this, event, std::move(target), std::move(location),
                           UserGestureIndicator::CurrentToken());
  timer->StartOneShot(TimeDelta(), FROM_HERE);
  timer->PauseIfNeeded();
  probe::AsyncTaskScheduled(document(), "postMessage", timer);
  post_message_timers_.insert(timer);
}
