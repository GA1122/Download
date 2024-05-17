void RenderThreadImpl::IdleHandler() {
  bool run_in_foreground_tab = (widget_count_ > hidden_widget_count_) &&
                               content::GetContentClient()->renderer()->
                                   RunIdleHandlerWhenWidgetsHidden();
  if (run_in_foreground_tab) {
    IdleHandlerInForegroundTab();
    return;
  }
#if !defined(OS_MACOSX) && defined(USE_TCMALLOC)
  MallocExtension::instance()->ReleaseFreeMemory();
#endif

  v8::V8::IdleNotification();

  ScheduleIdleHandler(idle_notification_delay_in_ms_ +
                      1000000 / (idle_notification_delay_in_ms_ + 2000));

  FOR_EACH_OBSERVER(RenderProcessObserver, observers_, IdleNotification());
}
