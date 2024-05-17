 void RenderThread::IdleHandler() {
  if (!widget_count_ || hidden_widget_count_ < widget_count_)
    return;

#if defined(OS_WIN)
  MallocExtension::instance()->ReleaseFreeMemory();
#endif

  LOG(INFO) << "RenderThread calling v8 IdleNotification for " << this;
  v8::V8::IdleNotification();

  idle_notification_delay_in_s_ +=
      1.0 / (idle_notification_delay_in_s_ + 2.0);

  MessageLoop::current()->PostDelayedTask(FROM_HERE,
      task_factory_->NewRunnableMethod(&RenderThread::IdleHandler),
      static_cast<int64>(floor(idle_notification_delay_in_s_)) * 1000);
}
