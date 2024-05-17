void RenderThreadImpl::WidgetHidden() {
  DCHECK_LT(hidden_widget_count_, widget_count_);
  hidden_widget_count_++;

  if (widget_count_ && hidden_widget_count_ == widget_count_) {
    base::internal::DiscardableMemoryEmulated::
        ReduceMemoryUsageUntilWithinLimit(
            kEmulatedDiscardableMemoryBytesToKeepWhenWidgetsHidden);

    if (GetContentClient()->renderer()->RunIdleHandlerWhenWidgetsHidden())
      ScheduleIdleHandler(kInitialIdleHandlerDelayMs);
  }
}
