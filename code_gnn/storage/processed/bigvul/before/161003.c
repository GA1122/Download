void ChromeClientImpl::SetHasScrollEventHandlers(LocalFrame* frame,
                                                 bool has_event_handlers) {
  if (!frame)
    return;

  WebFrameWidgetBase* widget =
      WebLocalFrameImpl::FromFrame(frame)->LocalRoot()->FrameWidget();
  DCHECK(!widget || widget->GetLayerTreeView() || !has_event_handlers);
  if (widget && widget->GetLayerTreeView())
    widget->GetLayerTreeView()->SetHaveScrollEventHandlers(has_event_handlers);
}
