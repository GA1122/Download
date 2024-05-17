WebWidget* RenderViewImpl::createPopupMenu(WebKit::WebPopupType popup_type) {
  RenderWidget* widget =
      RenderWidget::Create(routing_id_, popup_type, screen_info_);
  return widget->webwidget();
}
