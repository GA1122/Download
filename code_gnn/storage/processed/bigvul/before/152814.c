  void UpdateAllLifecyclePhasesExceptPaint() {
    WebView().MainFrameWidget()->UpdateLifecycle(
        WebWidget::LifecycleUpdate::kPrePaint,
        WebWidget::LifecycleUpdateReason::kTest);
  }
