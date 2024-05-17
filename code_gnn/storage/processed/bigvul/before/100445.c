void RenderThread::WidgetHidden() {
  DCHECK(hidden_widget_count_ < widget_count_);
  hidden_widget_count_++ ;
  if (widget_count_ && hidden_widget_count_ == widget_count_) {
    idle_notification_delay_in_s_ = kInitialIdleHandlerDelayS;

    MessageLoop::current()->PostDelayedTask(FROM_HERE,
        task_factory_->NewRunnableMethod(&RenderThread::IdleHandler),
        static_cast<int64>(floor(idle_notification_delay_in_s_)) * 1000);
  }
}
