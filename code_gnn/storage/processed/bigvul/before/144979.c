void RenderWidgetHostViewAura::SetPopupChild(
    RenderWidgetHostViewAura* popup_child_host_view) {
  popup_child_host_view_ = popup_child_host_view;
  event_handler_->SetPopupChild(
      popup_child_host_view,
      popup_child_host_view ? popup_child_host_view->event_handler() : nullptr);
}
