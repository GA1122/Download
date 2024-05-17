void RenderThread::IdleHandler() {
#if !defined(OS_MACOSX) && defined(USE_TCMALLOC)
  MallocExtension::instance()->ReleaseFreeMemory();
#endif

  v8::V8::IdleNotification();

  ScheduleIdleHandler(idle_notification_delay_in_s_ +
                      1.0 / (idle_notification_delay_in_s_ + 2.0));

  FOR_EACH_OBSERVER(RenderProcessObserver, observers_, IdleNotification());
}
