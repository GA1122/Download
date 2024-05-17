InputEventAckWaiter::InputEventAckWaiter(RenderWidgetHost* render_widget_host,
                                         InputEventAckPredicate predicate)
    : render_widget_host_(render_widget_host),
      predicate_(predicate),
      event_received_(false) {
  render_widget_host_->AddInputEventObserver(this);
}
