WebWidget* RenderViewImpl::CreatePopup(blink::WebLocalFrame* creator) {
  RenderWidget* popup_widget = RenderWidget::CreateForPopup(
      this, GetWidget()->compositor_deps(), GetWidget()->screen_info());
  if (!popup_widget)
    return nullptr;
  popup_widget->ApplyEmulatedScreenMetricsForPopupWidget(GetWidget());
  return popup_widget->GetWebWidget();
}
