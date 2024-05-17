WebEventListenerProperties ChromeClientImpl::EventListenerProperties(
    LocalFrame* frame,
    WebEventListenerClass event_class) const {
  if (!frame)
    return WebEventListenerProperties::kNothing;

  WebFrameWidgetBase* widget =
      WebLocalFrameImpl::FromFrame(frame)->LocalRoot()->FrameWidget();

  if (!widget || !widget->GetLayerTreeView())
    return WebEventListenerProperties::kNothing;
  return widget->GetLayerTreeView()->EventListenerProperties(event_class);
}
