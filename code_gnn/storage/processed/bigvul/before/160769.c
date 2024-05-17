WebWidget* RenderViewImpl::CreatePopup(blink::WebLocalFrame* creator,
                                       blink::WebPopupType popup_type) {
  RenderWidget* widget = RenderWidget::CreateForPopup(
      this, compositor_deps_, popup_type, screen_info_,
      creator->GetTaskRunner(blink::TaskType::kUnthrottled));
  if (!widget)
    return nullptr;
  if (screen_metrics_emulator_) {
    widget->SetPopupOriginAdjustmentsForEmulation(
        screen_metrics_emulator_.get());
  }
  return widget->GetWebWidget();
}
