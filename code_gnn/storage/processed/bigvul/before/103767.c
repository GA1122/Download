void RenderThread::WidgetHidden() {
  DCHECK(hidden_widget_count_ < widget_count_);
  hidden_widget_count_++;

  if (!content::GetContentClient()->renderer()->
          RunIdleHandlerWhenWidgetsHidden()) {
    return;
  }

  if (widget_count_ && hidden_widget_count_ == widget_count_)
    ScheduleIdleHandler(kInitialIdleHandlerDelayS);
}
