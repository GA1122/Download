void ChromeClientImpl::SetEventListenerProperties(
    LocalFrame* frame,
    WebEventListenerClass event_class,
    WebEventListenerProperties properties) {
  if (!frame)
    return;

  WebLocalFrameImpl* web_frame = WebLocalFrameImpl::FromFrame(frame);
  WebFrameWidgetBase* widget = web_frame->LocalRoot()->FrameWidget();
  if (!widget) {
    DCHECK(properties == WebEventListenerProperties::kNothing);
    return;
  }

  WebWidgetClient* client = widget->Client();
  if (WebLayerTreeView* tree_view = widget->GetLayerTreeView()) {
    tree_view->SetEventListenerProperties(event_class, properties);
    if (event_class == WebEventListenerClass::kTouchStartOrMove) {
      client->HasTouchEventHandlers(
          properties != WebEventListenerProperties::kNothing ||
          tree_view->EventListenerProperties(
              WebEventListenerClass::kTouchEndOrCancel) !=
              WebEventListenerProperties::kNothing);
    } else if (event_class == WebEventListenerClass::kTouchEndOrCancel) {
      client->HasTouchEventHandlers(
          properties != WebEventListenerProperties::kNothing ||
          tree_view->EventListenerProperties(
              WebEventListenerClass::kTouchStartOrMove) !=
              WebEventListenerProperties::kNothing);
    }
  } else {
    client->HasTouchEventHandlers(true);
  }
}
