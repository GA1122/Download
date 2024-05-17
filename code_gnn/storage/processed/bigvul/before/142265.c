void AddToWidgetInputEventObservers(
    content::RenderWidgetHost* widget_host,
    content::RenderWidgetHost::InputEventObserver* observer) {
  widget_host->RemoveInputEventObserver(observer);
  widget_host->AddInputEventObserver(observer);
}
